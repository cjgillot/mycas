/*
 * monomial.hxx
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

#ifndef MONOMIAL_HXX_
#define MONOMIAL_HXX_

#include "stdlib.hxx"
#include "operators.hxx"

#include "algebra/integer.hxx"

namespace poly {
namespace sparse {

template<class K>
struct monomial
: boost::arithmetic1<monomial<K>
, boost::multiplicative2<monomial<K>, K
, operators::ordered<monomial<K>
, operators::printable<monomial<K>
> > > >
{
  typedef algebra::integer Z;

  typedef K k;
  typedef Z z;

private:
  k coef;
  z expo;

public:
  inline
  monomial() {}
  inline
  monomial(const monomial &m)
  : coef(m.coef), expo(m.expo) {}
  inline monomial &
  operator=(const monomial &m) {
    coef = m.coef; expo = m.expo;
    return *this;
  }

  inline
  monomial(const k &c, const z &e)
  : coef(c), expo(e) {}
  explicit inline
  monomial(const k &c)
  : coef(c) {}

  inline
  ~monomial() {}

  static inline monomial
  var(const z &e)
  { return monomial(algebra::one<K>(), e); }

public: /// ring objects
  static const monomial zero;
  static const monomial one;

  inline bool
  null() const
  { return algebra::null(coef); }
  inline bool
  unit() const
  { return algebra::null(expo) && algebra::unit(coef); }

  inline const z &
  deg() const
  { return expo; }

public: /// printing
  template<class S>
  inline void
  print(S &ios) const {
    ios << coef << '@' << expo;
  }

public: /// operations
  inline monomial &
  operator+=(const monomial &o) {
    if(null()) return *this = o;
    assert(expo == o.expo);
    coef += o.coef;
    return *this;
  }
  inline monomial &
  operator-=(const monomial &o) {
    if(null()) return *this = o;
    assert(expo == o.expo);
    coef -= o.coef;
    return *this;
  }
  inline monomial &
  ineg() {
    algebra::ineg(coef);
    return *this;
  }
  inline monomial
  operator-() const {
    return monomial(*this).ineg();
  }

  inline monomial &
  operator*=(const k &o) {
    coef *= o;
    return *this;
  }
  inline monomial &
  operator/=(const k &o) {
    coef /= o;
    return *this;
  }

  inline monomial &
  operator*=(const monomial &o) {
    coef *= o.coef;
    expo += o.expo;
    return *this;
  }
  inline monomial &
  operator/=(const monomial &o) {
    assert(coef >= o.coef);
    coef /= o.coef;
    expo -= o.expo;
    return *this;
  }

public: /// comparison
  static inline int
  compare(const monomial &a, const monomial &b) {
    return algebra::compare(a.expo, b.expo);
  }
  static inline int
  deep_compare(const monomial &a, const monomial &b) {
    int cmpe = algebra::compare(a.expo, b.expo);
    if(cmpe != 0) return cmpe;
    return algebra::compare(a.coef, b.coef);
  }
};

template<class K>
const monomial<K> monomial<K>::zero;

template<class K>
const monomial<K> monomial<K>::one (
  algebra::one<K>(),
  algebra::zero<monomial<K>::Z>()
);

}} // poly::sparse

#endif /* MONOMIAL_HXX_ */
