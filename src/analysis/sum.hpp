/*
 * sum.hpp
 *
 *  Created on: 20 juin 2011
 *      Author: k1000
 */

#ifndef ADD_HPP_
#define ADD_HPP_

#include "analysis/expairseq.hpp"
#include "analysis/prod.hpp"

#include "util/final.hpp"

namespace analysis {

class sum;

DECLARE_FINAL_CLASS(sum)

/*!
 * \brief Addition class
 *
 * This \c expairseq-derived class represents
 * sums as a linear combination of multiplications,
 * whose coefficients are pure \c number's.
 *
 * Printing : (+ c m1 m2 m3 ...)
 */
class sum: FINAL_CLASS(sum)
, public expairseq<sum, prod> {

  DEFINE_CONST_VISITABLE()

private:
  //! \brief Base class type
  typedef expairseq<sum, prod> super;

public:
  struct ep;

protected:
  //! \brief Copy constructor (for clone)
  sum(const sum &);

public:
  //! \brief Destructor
  virtual ~sum();

protected:
  //! \brief Number constructor
  explicit sum(const number &);

  //! \brief Multiplication constructor
  sum(const number &, const epair &);

  using super::add_t;
  using super::sub_t;
  using super::neg_t;
  using super::sca_t;

  template<class Tag>
  sum(const sum &a, const sum &b, Tag);
  sum(const sum &a, neg_t);
  sum(const sum &a, const number &n, sca_t);

public:
  sum* clone() const
  { return new sum(*this); }

public:
  virtual const sum* as_sum() const
  { return this; }

public:
  bool null() const;
  expr eval(unsigned) const;
  expr diff(const symbol&, unsigned=1) const;

public:
  sum* add(const sum &o) const;
  sum* sub(const sum &o) const;
  sum* neg() const;

  sum* mul(const number &n) const;

private:
  void print_base(std::basic_ostream<char> &os) const
  { os << '+'; }

public:
  static sum*
  from_1basic(const basic*);

  static sum*
  from_numeric(const numeric*);
};

}

#endif /* ADD_HPP_ */
