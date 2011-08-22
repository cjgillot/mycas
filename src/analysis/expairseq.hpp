/*
 * expairseq.hpp
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#ifndef EXPAIRSEQ_HPP_
#define EXPAIRSEQ_HPP_

#include<boost/intrusive_ptr.hpp>

#include "analysis/number.hpp"

namespace analysis {

template<class, class>
class expairseq;

}

#include "analysis/expairseq/handle.hpp"
#include "analysis/expairseq/poly.hpp"

namespace analysis {

/*!
 * \brief Implementation class for \c sum and \c mul
 *
 * This CRTP abstract class is used as a common implementation for
 * both \c sum and \c mul.
 *
 * The object is represented as a pair coefficient-polynomial,
 * with a scalar coefficient and a polynomial of \c Mono over the scalar ring.
 *
 * The polynomial is shared and its hash value is computed during
 * construction, in order to save comparison times.
 *
 * \param Impl : the implemented class
 * \param Mono : the monomial class
 */
template<class Impl, class Mono>
class expairseq
: public basic {

  // expairseq is an implemetation base class -> not registered
  // REGISTER_CLASS( expairseq, basic )

protected:
  typedef typename Mono::handle epair;
  typedef epseq::detail::unsafe_vector<epair> poly_t;

  //! \brief Handle class
  typedef epseq::handle<Impl, Mono> handle;
  friend class epseq::handle<Impl, Mono>;

protected:
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
  std::size_t hash() const;

  util::cmp_t partial_compare(const expairseq &) const;
  util::cmp_t compare_same_type(const basic &) const;

public:
  void print(std::basic_ostream<char> &os) const;

  bool has(const symbol &) const;

private: // member data
  //! \brief Constant coefficient
  number m_coef;

  //! \brief Shared polynomial vector
  //! \invariant an empty polynomial is represented by \c nullptr
  boost::intrusive_ptr<const poly_t> m_poly;

  //! \brief Hash value of \c m_poly
  std::size_t m_hash;
};

} // namespace analysis

#include "analysis/expairseq/handle.ipp"

#endif /* EXPAIRSEQ_HPP_ */
