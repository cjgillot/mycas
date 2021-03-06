#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"

#include "analysis/function/exprseq.hpp"

#include "container/unsafe_array.hpp"

#include "util/assert.hpp"

namespace analysis {
namespace detail {

template<class E, unsigned N>
struct choose_container
{
  typedef container::unsafe_array<E, N> type;
};

template<class E>
struct choose_container<E, 0>
{
  typedef std::vector<E> type;
};

template<unsigned N>
struct construction
{
  typedef typename choose_container<expr, N>::type cont_t;
  static typename cont_t::unsafe_tag tag() { return typename cont_t::unsafe_tag(); }

  static void reserve( unsigned, cont_t & )
  {}

  static void construct( unsigned n, cont_t &cont, const expr &val )
  {
    cont.get_allocator().construct( cont.begin() + n, val );
  }

  template<std::size_t I, typename Arg0, typename... Args>
  static void construct_loop( cont_t &cont, Arg0&& a0, Args&& ...rest )
  {
    cont.get_allocator().construct( cont.begin() + I, a0 );
    construct_loop<I+1>( cont, std::forward<Args>(rest)... );
  }

  template<std::size_t I>
  static void construct_loop(cont_t&) {}
};

template<>
struct construction<0>
{
  typedef typename choose_container<expr, 0>::type cont_t;
  static typename cont_t::allocator_type tag() { return typename cont_t::allocator_type(); }

  static void reserve( unsigned n, cont_t &cont )
  { cont.reserve( n ); }

  static void construct( unsigned, cont_t &cont, const expr &value )
  {
    cont.push_back( value );
  }

  template<typename Arg0, typename... Args>
  static void construct_loop( cont_t &cont, Arg0&& a0, Args&& ...rest )
  {
    cont.push_back( a0 );
    construct_loop( cont, std::forward<Args>(rest)... );
  }
};

} // namespace detail

struct func_id
{
  symbol name;
  std::size_t hash; // != name.hash()
};

class function_base
{
protected:
  inline function_base(const func_id &i)
  : m_id(i) {}
  virtual ~function_base() {}

public:
  const func_id &id() const
  { return m_id; }

private:
  const func_id &m_id;
};

template<unsigned N>
class function
: public exprseq< typename detail::choose_container<expr, N>::type, true >
, public function_base
{
  REGISTER_STATIC_CLASS( function<N>, basic, FUNCTION_RTTI )

private:
  typedef typename function::exprseq_ super;

public:
  struct iterator_tag {};

  template<class InputIterator>
  function(const func_id &i, InputIterator&& b, InputIterator&& e, iterator_tag)
  : super( std::forward<InputIterator>(b), std::forward<InputIterator>(e) )
  , function_base(i) {}

  template<typename... Args>
  function(const func_id& i, Args&& ...args)
  : super( detail::construction<N>::tag() )
  , function_base( i )
  {
    typedef detail::construction<N> op_t;
    op_t::reserve( sizeof...(Args), this->super::get_container() );
    op_t::template construct_loop<0>( this->super::get_container(), std::forward<Args>(args)... );
  }

protected:
  //! \brief Argument list sorting
  void sort();

  /*!\brief Signed sorting
   *
   * This function sorts the arguments and
   * additionally computes the sign of
   * the permutation (1 or -1).
   *
   * This function should only be used when the
   * permutation sign is significant,
   * since it has a far greater time constant.
   *
   * \return \c false for 1, \c true for -1
   *
   * \see sort()
   */
  bool sign_sort();

public:
  //! \brief Argument access
  const expr &arg(unsigned i) const
  { return super::at(i); }

  //! \brief Argument access
  template<unsigned I>
  const expr &arg() const
  {
    STATIC_ASSERT( I < N );
    return super::operator[](I);
  }

private: // hide some of super members
  using super::operator[];
  using super::at;

protected:
  using super::print_children;

public:
  function* clone() const
  {
    return new function( *this );
  }

  void print(std::basic_ostream<char> &os) const
  {
    os << '(' << id().name << ' ';
    print_children(os);
    os << ')';
  }

protected: // implemented in "derivative.hpp"
  expr differentiate(const symbol &s) const;
};

}

#endif
