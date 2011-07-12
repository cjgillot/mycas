/*
 * expairseq.hxx
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#ifndef EXPAIRSEQ_HXX_
#define EXPAIRSEQ_HXX_

#include "numeric.hxx"
#include "util/pimpl.hxx"

#include "expair.hxx"

#include "polynomial/sparsefwd.hxx"

namespace analysis {


template<class Traits, class MonoTraits>
struct expairseq_traits;


template<class Traits, class MonoTraits>
class expairseq
: public basic {

public:
  //! \brief Actual traits class passed to expair
  typedef expairseq_traits<Traits,MonoTraits> traits;

  //! \brief Pair type
  typedef expair<traits> impl_t;

protected:
  typedef typename traits::epair epair;
  typedef typename traits::coef_type coef_type;

private:
  //! \brief Pair implementation member
  impl_t m_impl;

  typedef typename traits::poly_t poly_t;

public:
  //! \brief Coercion from expair
  expairseq(const impl_t &i);
  //! \brief Coercion to expair
  operator impl_t() const;

  //! \}

protected:
  //! \brief Copy constructor
  expairseq(const expairseq &o);
  //! \brief Non-throwing swap
  void swap(expairseq &o);

private:
  // disabled
  expairseq();
  expairseq &operator=(const expairseq &);

public:
  explicit
  expairseq(const coef_type &n);
  expairseq(const coef_type &n, const epair &p);
  expairseq(const coef_type &n, const poly_t* i);
  ~expairseq();

  virtual expairseq* clone() const = 0;

protected:
  // shall be called after any modifying operation
  void canonicalize();

  // for the derived, we're and addition
  // for expair, we're a multiplication
  expairseq &iadd(const expairseq &);
  expairseq &isub(const expairseq &);
  expairseq &ineg();

  // scalar multiplication
  expairseq &imul(const coef_type &);

  // access
  const coef_type &coef() const;
  bool is_empty() const;
  bool is_mono() const;
  const epair &mono() const;

private:
  std::size_t calc_hash() const;

public:
  int compare_same_type(const basic &) const;
  void print(std::basic_ostream<char> &os) const;
};


template<class Traits, class MonoTraits>
struct expairseq_traits {
  typedef expairseq<Traits, MonoTraits> type;

  typedef expair<MonoTraits> epair;
  typedef poly::sparse::poly<epair> poly_t;

  struct ep;

  typedef typename Traits::coef_type coef_type;
  typedef util::cow_pimpl<poly_t> rest_type;
};


}

//#include "expairseq.ixx"

#endif /* EXPAIRSEQ_HXX_ */
