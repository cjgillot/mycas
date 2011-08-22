#ifndef RTTI_VISITOR_IPP
#define RTTI_VISITOR_IPP

#include "util/attribute.hpp"

#include "rtti/visitor.hpp"
#include "rtti/holder.hpp"

#include <algorithm>

namespace rtti {
namespace visitor {
namespace detail {

/*!
 * \brief Trampoline function
 *
 * This template structure provides the static member
 * function \c invoker that will act as a trampoline
 * whose address will be taken.
 *
 * Direct template function didn't work because
 * the symbol was not emitted when just taking the address.
 */
template<
  class Visitor
, class Derived
, class Base
>
struct trampoline {
  //! \brief The function
  static void invoker(
    const Visitor &v
  , const Base &b
  )
  {
    v.visit( static_cast<const Derived&>( b ) );
  }
};

//! \brief Helper structure for initialization
template<class Visitor, class Base>
struct init_helper {
  //! \brief Current node in the table
  vtable_node* cur;

  //! \brief Called function
  template<class T>
  inline void apply()
  {
    typedef typename ::rtti::detail::get_holder<T>::type h;
    h::assert_initialized();

    cur->id = h::get_id();
    cur->invoker = reinterpret_cast< vtable_node::invoker_t >(
      &trampoline<Visitor, T, Base>::invoker
    );

    assert( cur->invoker );

    ++cur;
  }
};

//! \brief Sorting predicate : with respect to the id
struct sort_pred
{
  inline bool ATTRIBUTE_PURE
  operator()( const vtable_node &a, const vtable_node &b )
  { return a.id < b.id; }
};

} // namespace detail

template<class V>
inline void
holder<V>::initialize() {
  typedef typename Types::head B;

  detail::init_helper<V, B> h = { vtable };
  Types::for_each( h );

  std::sort( vtable, vtable + N, detail::sort_pred() );

  assert( vtable[0].invoker );
}

template<class V>
detail::vtable_node holder<V>::vtable[ N ] = {};

template<class V>
detail::initializer_t<holder<V> > holder<V>::initializer;

}} // namespace rtti::visitor

#endif
