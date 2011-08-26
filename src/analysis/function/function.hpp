#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"

#include "util/foreach.hpp"
#include "util/assert.hpp"

#include "algebra/compare.hpp"

#include "container/unsafe_array.hpp"

namespace analysis {

template<unsigned N>
class function
: public basic
{
  STATIC_ASSERT( N > 0 );

  symbol m_name;
  container::unsafe_array<expr, N> m_args;

protected:
  template<class InputIterator>
  function(const std::string &name,
    const InputIterator &b, const InputIterator &e);

  function(const function &);

 ~function();

protected:
  //! \brief Argument list sorting
  void sort();

  /*!
   * \brief Signed sorting
   *
   * This function sorts the arguments and
   * additionally computes the sign of
   * the permutation (1 or -1).
   *
   * This function should only be used when the
   * permutation sign is signficant,
   * since it has a far greater time constant.
   *
   * \return \c false for 1, \c true for -1
   *
   * \see sort()
   */
  bool sign_sort();

  //! \brief Argument access
  const expr &arg(unsigned i) const
  { return m_args.at(i); }

  //! \brief Argument access
  template<unsigned I>
  const expr &arg() const
  { STATIC_ASSERT( I < N ); return m_args[I]; }

private:
  std::size_t hash() const
  {
    std::size_t seed = m_name.hash();
    foreach(const expr &e, m_args)
      boost::hash_combine(seed, e.hash());
    return seed;
  }

  util::cmp_t
  compare_same_type(const basic &o_) const
  {
    const function &o = static_cast<const function&>(o_);

    util::cmp_t c
      = symbol::compare(m_name, o.m_name);
    if(c) return c;

    return algebra::range_compare(m_args, o.m_args, expr::compare);
  }

  bool has(const symbol &s) const
  {
    foreach(const expr &e, m_args)
      if( e.has(s) )
        return true;
  }

public:
  void print(std::basic_ostream<char> &os) const {
    os << '(' << m_name;
    foreach(const expr &e, m_args)
      os << ' ' << e;
    os << ')';
  }
};

template<>
class function<1>
: public basic
{
  symbol m_name;
  expr   m_arg;

public:
  function(const std::string &name, const expr &a)
  : m_name(name), m_arg(a) {}

protected:
  //! \brief Argument list sorting
  void sort() {}

  /*!
   * \brief Signed sorting
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

private:
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
