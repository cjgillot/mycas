#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"

#include<boost/array.hpp>
#include<boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include "algebra/compare.hpp"

namespace analysis {

template<unsigned N>
class function
: public basic
{
  BOOST_STATIC_ASSERT( N > 0 );

  symbol m_name;
  std::vector<expr> m_args;

protected:
  template<class InputIterator>
  function(const std::string &name,
    const InputIterator &b, const InputIterator &e);

  function(const function &);
  function &operator=(const function &);
  void swap(function &);

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
   * since it has a far grater time constant.
   *
   * \return \c false for 1, \c true for -1
   *
   * \see sort()
   */
  bool sign_sort();

  //! \brief Argument access
  const expr &arg(unsigned n) const
  { assert( n < N ); return m_args[n]; }

private:
  std::size_t calc_hash() const
  {
    std::size_t seed = m_name.hash();
    foreach(const expr &e, m_args)
      boost::hash_combine(seed, e.hash());
    return seed;
  }

  int compare_same_type(const basic &o_) const
  {
    const function &o = static_cast<const function&>(o_);

    int c = symbol::compare(m_name, o.m_name);
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

  void swap(function &o)
  {
    m_name.swap(o.m_name);
    m_arg.swap(o.m_arg);
  }

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
  const expr &arg(unsigned n = 0) const
  { assert( n == 0 ); return m_arg; }

private:
  std::size_t calc_hash() const
  {
    std::size_t seed = m_name.hash();
    boost::hash_combine(seed, m_arg.hash());
    return seed;
  }

  int compare_same_type(const basic &o_) const
  {
    const function &o = static_cast<const function&>(o_);

    int c = symbol::compare(m_name, o.m_name);
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
