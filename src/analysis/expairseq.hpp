/*
 * expairseq.hpp
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#ifndef EXPAIRSEQ_HPP_
#define EXPAIRSEQ_HPP_

#include "util/pimpl.hpp"

#include "analysis/number.hpp"

namespace analysis {

template<class Impl, class Mono>
class expairseq
: public basic {

  DEFINE_CONST_VISITABLE()

protected:
  typedef typename Mono::handle epair;
  typedef std::vector<epair> poly_t;

  //! \brief Handle class
  struct handle;

protected:
  //! \brief Copy constructor
  expairseq(const expairseq &o);
  //! \brief Non-throwing swap
  void swap(expairseq &o);

  //! \brief Constant construction
  explicit
  expairseq(const number &n);

  //! \brief Constant-monomial construction
  expairseq(const number &n, const epair &p);

  //! \brief Detructor
  ~expairseq();

private:
  //! \brief Virtual constructor
  virtual expairseq* clone() const = 0;

protected:
  /*!\name Operation constructors
   *
   * Given two \c expairseq, these constructors build
   * a merge depending on the call tag.
   *
   * \{
   */
  //@{
  struct add_t {};
  struct sub_t {};
  struct neg_t {};
  struct sca_t {};

  expairseq(const expairseq &, const expairseq &, add_t);
  expairseq(const expairseq &, const expairseq &, sub_t);
  expairseq(const expairseq &, neg_t);

  expairseq(const expairseq &, const number &, sca_t);
  //@}
  /*!\}*/

protected:
  // access
  const number &coef() const;
  bool is_empty() const;
  bool is_mono() const;
  const epair &mono() const;

public:
  /*!\name Range operations
   * \{
   */
  //@{
  typedef typename poly_t::const_iterator         const_iterator;
  typedef typename poly_t::const_reverse_iterator const_reverse_iterator;

  const_iterator begin() const { return m_poly ? m_poly->begin() : const_iterator(); }
  const_iterator end()   const { return m_poly ? m_poly->end()   : const_iterator(); }

  const_reverse_iterator rbegin() const { return m_poly ? m_poly->rbegin() : const_reverse_iterator(); }
  const_reverse_iterator rend()   const { return m_poly ? m_poly->rend()   : const_reverse_iterator(); }
  //@}
  /*!\}*/

private:
  std::size_t calc_hash() const;

public:
  int partial_compare(const expairseq &) const;
  int compare_same_type(const basic &) const;
  void print(std::basic_ostream<char> &os) const;

  virtual void print_base(std::basic_ostream<char> &) const = 0;

  bool has(const symbol &) const;

private: // member data
  number m_coef;
  boost::shared_ptr<poly_t> m_poly;

  // contains the hash value of {m_poly}
  std::size_t m_hash;
};

template<class I, class M>
struct expairseq<I,M>::handle {

  handle(const expairseq* p)
  : m_ptr(p) { assert(p); }

  handle(const handle &o)
  : m_ptr(o.m_ptr) {}

  handle &operator=(const handle &o) {
    m_ptr = o.m_ptr;
    return *this;
  }

  ~handle() {}

  operator expr() const
  { return expr(m_ptr.get()); }

  handle operator+(const handle &o) const {
    assert(compare(*this, o) == 0);
    return chg_coef(m_ptr->m_coef + o.m_ptr->m_coef);
  }
  handle operator-(const handle &o) const {
    assert(compare(*this, o) == 0);
    return chg_coef(m_ptr->m_coef - o.m_ptr->m_coef);
  }
  handle operator-() const {
    return chg_coef(- m_ptr->m_coef);
  }
  handle sca(const number &n) const {
    return chg_coef(m_ptr->m_coef * n);
  }

  bool null() const
  { return m_ptr->m_coef.null(); }

  static int compare(const handle &a, const handle &b)
  { return a.m_ptr->partial_compare(*b.m_ptr); }
  static int deep_compare(const handle &a, const handle &b)
  { return basic::compare(*a.m_ptr, *b.m_ptr); }

  std::size_t hash() const
  { return m_ptr->get_hash(); }

  template<class S>
  void print(S &os) const
  { m_ptr->print(os); }

  const expairseq* ptr() const
  { return m_ptr.get(); }

private:
  expairseq* chg_coef(const number &n) const {
    expairseq* ret = m_ptr->clone();
    ret->m_coef = n;
    ret->basic::unhash();
    return ret;
  }

  boost::intrusive_ptr<const expairseq> m_ptr;
};

} // namespace analysis

#endif /* EXPAIRSEQ_HPP_ */
