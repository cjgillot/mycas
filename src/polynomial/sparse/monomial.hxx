/*
 * monomial.hxx
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

#ifndef MONOMIAL_HXX_
#define MONOMIAL_HXX_

#include "stdlib.hxx"

#include "algebra/integer.hxx"

namespace poly {
namespace sparse {

template<class K>
struct monomial {
  typedef algebra::integer Z;

  typedef K k;
  typedef Z z;

private:
  k coef;
  z expo;

public:
  monomial(const k &c, const z &e);

  explicit
  monomial(const z &e);

  monomial();
  monomial(const monomial &m);
  monomial &operator=(const monomial &);

  ~monomial();

  static monomial zero;
  static monomial one;

  bool null() const;
  bool unit() const;

  const z &deg() const;

  monomial &operator+=(const monomial &o);
  monomial &operator-=(const monomial &o);

  monomial &operator*=(const k &o);
  monomial &operator/=(const k &o);

  monomial &operator*=(const monomial &o);
  monomial &operator/=(const monomial &o);

  monomial &ineg();
  monomial  neg() const;

  static int compare(const monomial &a, const monomial &b);
  static int deep_compare(const monomial &a, const monomial &b);
};

}} // poly::sparse

#include "monomial.inl"

#endif /* MONOMIAL_HXX_ */
