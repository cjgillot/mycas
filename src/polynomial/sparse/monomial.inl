/*
 * monomial.hxx
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

#ifndef MONOMIAL_INL_
#define MONOMIAL_INL_

#include "stdlib.hxx"

#include "algebra/utils.hxx"

#include "monomial.hxx"

namespace poly {
namespace sparse {

template<class K>
inline
monomial<K>::monomial(const k &c, const z &e)
: coef(c)
, expo(e)
{}

template<class K>
inline
monomial<K>::monomial(const z &e)
: coef(algebra::one<K>())
, expo(e)
{}

template<class K>
inline
monomial<K>::monomial()
: coef(algebra::zero<K>())
, expo(algebra::zero<Z>())
{}

template<class K>
inline
monomial<K>::monomial(const monomial &m)
: coef(m.coef)
, expo(m.expo)
{}

template<class K>
inline monomial<K> &
monomial<K>::operator=(const monomial &m) {
  coef = m.coef;
  expo = m.expo;
  return *this;
}

template<class K>
inline
monomial<K>::~monomial()
{}

template<class K>
monomial<K> monomial<K>::zero;

template<class K>
monomial<K> monomial<K>::one (
  algebra::one<K>(),
  algebra::zero<monomial<K>::Z>()
);

template<class K>
inline bool
monomial<K>::null() const {
  return algebra::null(coef);
}

template<class K>
inline bool
monomial<K>::unit() const {
  return algebra::unit(coef) && algebra::null(expo);
}

template<class K>
inline const typename monomial<K>::z &
monomial<K>::deg() const {
  return expo;
}

template<class K>
inline monomial<K> &
monomial<K>::operator+=(const monomial &o) {
  assert(expo == o.expo);
  coef += o.coef;
  return *this;
}

template<class K>
inline monomial<K> &
monomial<K>::operator-=(const monomial &o) {
  assert(expo == o.expo);
  coef -= o.coef;
  return *this;
}

template<class K>
inline monomial<K> &
monomial<K>::ineg() {
  coef = - coef;
  return *this;
}
template<class K>
inline monomial<K>
monomial<K>::neg() const {
  return monomial(- coef, expo);
}

template<class K>
inline monomial<K> &
monomial<K>::operator*=(const k &o) {
  coef *= o;
  return *this;
}
template<class K>
inline monomial<K> &
monomial<K>::operator/=(const k &o) {
  coef /= o;
  return *this;
}

template<class K>
inline monomial<K> &
monomial<K>::operator*=(const monomial &o) {
  coef *= o.coef;
  expo += o.expo;
  return *this;
}
template<class K>
inline monomial<K> &
monomial<K>::operator/=(const monomial &o) {
  assert(expo >= o.expo);
  coef /= o.coef;
  expo -= o.expo;
  return *this;
}

template<class K>
inline int
monomial<K>::compare(const monomial &a, const monomial &b) {
  return algebra::compare(a.expo, b.expo);
}

template<class K>
inline int
monomial<K>::deep_compare(const monomial &a, const monomial &b) {
  int cmpe = algebra::compare(a.expo, b.expo);
  if(cmpe != 0) return cmpe;
  return algebra::compare(a.coef, b.coef);
}

template<class K>
inline monomial<K>
operator+(monomial<K> a, const monomial<K> &b) {
  return a += b;
}

template<class K>
inline monomial<K>
operator-(monomial<K> a, const monomial<K> &b) {
  return a -= b;
}

template<class K>
inline monomial<K>
operator-(monomial<K> a) {
  return a.ineg();
}

template<class K>
inline monomial<K>
operator*(monomial<K> a, const K &b) {
  return a *= b;
}

template<class K>
inline monomial<K>
operator*(monomial<K> a, const monomial<K> &b) {
  return a *= b;
}

template<class K>
inline monomial<K>
operator/(monomial<K> a, const K &b) {
  return a /= b;
}

template<class K>
inline monomial<K>
operator/(monomial<K> a, const monomial<K> &b) {
  return a /= b;
}

}} // poly::sparse

#endif /* MONOMIAL_INL_ */
