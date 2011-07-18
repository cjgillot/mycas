#ifndef FUNCTION_HXX
#define FUNCTION_HXX

#include "stdlib.hpp"

#include "analysis/expr.hpp"

namespace analysis {

template<unsigned N>
class function
//: public basic
{
  std::string m_name;
  std::vector<expr> m_args;

private:
  // disabled
  function();

public:
  template<class InputIterator>
  function(const std::string &name,
    const InputIterator &, const InputIterator &);

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
   * otherwise use sort().
   *
   * \return false for 1, true for -1
   */
  bool sign_sort();

public:
  void print(std::basic_ostream<char> &os) {
    os << '(' << m_name;
    foreach(const expr &e, m_args)
      e.print(os << ' ');
    os << ')';
  }
};

}

#endif
