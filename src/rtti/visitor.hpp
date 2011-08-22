#ifndef RTTI_VISITOR_HPP
#define RTTI_VISITOR_HPP

#include "rtti/holder.hpp"
#include "rtti/interface.hpp"

#include <boost/preprocessor/seq/for_each.hpp>

namespace rtti {

/*!
 * \brief Efficient cyclic visitor based on RTTI
 */
namespace visitor {

//! \brief Miscellanous classes implementing type lists
namespace mpl {

//! \brief Type list construction
template<class H, class T>
struct cons {
  //! \brief Head type
  typedef H head;
  //! \brief Tail type
  typedef T tail;

  //! \brief List length
  enum { size = 1 + T::size };

  //! \brief Loop through listed types
  template<class F>
  static void for_each(F &f)
  {
    f.template apply<H>();
    T::for_each( f );
  }
};

//! \brief Empty list case
struct nil {
  //! \brief Null length
  enum { size = 0 };

  //! \brief No-op
  template<class F>
  static void for_each(F &)
  {}
};

} // namespace mpl

namespace detail {

//! \brief Node type for vtable
struct vtable_node {
  typedef void(*invoker_t)();

  rtti_type id;
  invoker_t invoker;
};

//! \brief Invoker retrieval function
vtable_node::invoker_t
fetch_node(
  const vtable_node* const beg
, const std::size_t vsz
, const ::rtti::detail::rtti_node* rt
) throw();

//! \brief Dummy class
//! This class allows forcing initialization
//! of the vtable before \c main begins.
template<class H>
struct initializer_t {
  //! \brief Constructor
  //! Ask for initialization
  initializer_t()   { H::assert_initialized(); }

  //! \brief No-op
  //! Calling this method once
  //! forces th compiler to emit code
  //! for the global instance
  void touch()      {}
};

} // namespace detail

//! \brief Instanciation class for visitor states
template< class Visitor >
struct holder {

private:
  //! \brief Visited types
  typedef typename Visitor::visited_type Types;

public:
  //! \brief Vtable size
  enum { N = Types::size };

  //! \brief Vtable static symbol
  static detail::vtable_node vtable [ N ];

  //! \brief Initialization routine
  static void
  assert_initialized() {
    initializer.touch();
    if( vtable[0].invoker == 0 )
      initialize();
  }

private:
  BOOST_STATIC_ASSERT( N > 0 );

  //! \brief Vtable initialization method
  //! This method is meant to be called by
  //! \c assert_initialized to avoid multiple
  //! initialization.
  //! Runtime complexity : N * lg N
  static void
  initialize();

  //! \brief Static initializer
  static detail::initializer_t<holder> initializer;
};

template< class Visitor, class Base >
void visit(
  const Visitor &v
, const Base &x
) {
  using namespace detail;;

  typedef holder< Visitor > H;

  H::assert_initialized();

  vtable_node::invoker_t inv = fetch_node(
    H::vtable, H::N, RTTI_NODE( x )
  );

  typedef void(*fnc_t)( const Visitor &v, const Base &b );

  ( *reinterpret_cast<fnc_t>(inv) )( v, x );
}

}} // namespace rtti::visitor

#define MAKE_VISITABLE( klass ) \
  template<class Visitor>       \
  inline void                   \
  accept( const Visitor &v )    \
  {                             \
    ::rtti::visitor::visit      \
      ( v, *this );             \
  }

#define VISITOR_TLIST( sequence )                            \
  BOOST_PP_SEQ_FOR_EACH( VISITOR_TLIST_BEGIN, _, sequence )  \
    ::rtti::visitor::mpl::nil                                      \
  BOOST_PP_SEQ_FOR_EACH( VISITOR_TLIST_END,   _, sequence )

#define VISITOR_TLIST_BEGIN( r, _, elem ) \
  ::rtti::visitor::mpl::cons< elem,

#define VISITOR_TLIST_END( r, _, elem ) >

#define MAKE_VISITOR( klass, base, visited )    \
private:                                        \
  typedef MAKE_TLIST( (base)visited )           \
    visited_type;                               \
  friend class ::rtti::visitor::holder< klass >;

#endif

// load inline implementation
#include "visitor.ipp"
