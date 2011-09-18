#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"

#include "analysis/function/exprseq.hpp"

#include "container/unsafe_array.hpp"

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

} // namespace detail

template<class Derived, unsigned N>
class function
: public exprseq< typename detail::choose_container<expr, N>::type >
{
private:
  typedef typename function::exprseq_ super;
  static symbol m_name;

protected:
  template<class InputIterator>
  function(const InputIterator &b, const InputIterator &e)
  : super( b, e ) {}

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
  using super::print_children;

public:
  void print(std::basic_ostream<char> &os) const
  {
    os << '(' << m_name << ' ';
    super::print_children( os );
    os << ')';
  }

private: // implemented in "derivative.hpp"
  expr differentiate(const symbol &s) const;
};

template<class Derived>
class function<Derived, 1>
: public basic
{
  static symbol m_name;
  expr   m_arg;

protected:
  function(const expr &a)
  : m_arg(a) {}

protected:
  //! \brief Argument list sorting
  void sort() {}

  /*!\brief Signed sorting
   *
   * This function sorts the arguments and
   * additionally computes the sign of
   * the permutation (1 or -1).
   *
   * This function should only be used when the
   * permutation sign is signficant,
   * since it has a far grater time constant.
   *
   * \return \c false for 1, \c true for -1
   *
   * \see sort()
   */
  bool sign_sort()
  { return false; }

  //! \brief Argument access
  const expr &arg(unsigned n) const
  { ASSERT( n == 0 ); return m_arg; }

  //! \brief Argument access
  template<unsigned I>
  const expr &arg() const
  { STATIC_ASSERT( I == 0 ); return m_arg; }

protected:
  std::size_t hash() const
  {
    std::size_t seed = m_name.hash();
    boost::hash_combine(seed, m_arg.hash());
    return seed;
  }

  util::cmp_t compare_same_type(const basic &o_) const
  {
    const function &o = static_cast<const function&>(o_);

    util::cmp_t c = symbol::compare(m_name, o.m_name);
    if(c) return c;

    return expr::compare(m_arg, o.m_arg);
  }

  bool has(const symbol &s) const
  { return m_arg.has(s); }

public:
  void print(std::basic_ostream<char> &os) const
  { os << '(' << m_name << ' ' << m_arg << ')'; }
};

}

#endif
