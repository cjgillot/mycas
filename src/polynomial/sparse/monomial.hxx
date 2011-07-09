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

/*!
 * \class monomial
 * \brief Default monomial class for sparse polynomials
 *
 * Here we define the monomial concept. monomials are :
 * - multipliable, dividable in all cases
 * - algebra::compare'able
 * - compatible deep_compare'able
 *   (ie. if algebra::compare says less, deep_compare must also)
 * - additionable ant subtractable if they algebra::compare equal
 * - printable
 */
template<class K, class Z=algebra::integer>
class monomial
: boost::arithmetic1<monomial<K>
, boost::multiplicative2<monomial<K>, K
, operators::ordered<monomial<K>
, operators::printable<monomial<K>
> > > > {
  /// represents [coef * X^expo]
  K coef;
  Z expo;

public:
  /*!
   * \brief Default Constructor
   */
  inline
  monomial() {}
  /*!
   * \brief Copy constructor
   */
  inline
  monomial(const monomial &m)
  : coef(m.coef), expo(m.expo) {}
  /*!
   * \brief Assignment operator
   */
  inline monomial &
  operator=(const monomial &m) {
    coef = m.coef; expo = m.expo;
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   */
  inline void
  swap(monomial &o) {
    std::swap(coef, o.coef);
    std::swap(expo, o.expo);
  }

  /*!
   * \brief Coefficient-exponent constructor
   */
  inline
  monomial(const K &c, const Z &e)
  : coef(c), expo(e) {}
  /*!
   * \brief Constant constructor
   */
  inline // explicit // FIXME : explicit or not ?
  monomial(const K &c)
  : coef(c) {}

  /*!
   * \brief Destructor
   */
  inline
  ~monomial() {}

  /*!
   * \brief Exponent constructor
   */
  static inline monomial
  var(const Z &e)
  { return monomial(algebra::one<K>(), e); }

public:
  /*!
   * \brief static zero polynomial
   */
  static const monomial zero;
  /*!
   * \brief static unity polynomial
   */
  static const monomial one;

  /*!
   * \brief nullity test
   *
   * constant time
   */
  inline bool
  null() const
  { return algebra::null(coef); }
  /*!
   * \brief unitity test
   *
   * constant time
   */
  inline bool
  unit() const
  { return algebra::null(expo) && algebra::unit(coef); }

  /*!
   * \brief Exponent (degree) accessor
   */
  inline const Z &
  deg() const
  { return expo; }

public:
  /*!
   * \brief in-place addition operator
   *
   * constant time
   */
  inline monomial &
  operator+=(const monomial &o) {
    if(null()) return *this = o;
    assert(expo == o.expo);
    coef += o.coef;
    return *this;
  }
  /*!
   * \brief in-place subtraction operator
   *
   * constant time
   */
  inline monomial &
  operator-=(const monomial &o) {
    if(null()) return *this = o;
    assert(expo == o.expo);
    coef -= o.coef;
    return *this;
  }

  /*!
   * \brief in-place negation operator
   *
   * constant time
   */
  inline monomial &
  ineg() {
    algebra::ineg(coef);
    return *this;
  }

  /*!
   * \brief in-place multiplication
   *
   * constant time
   */
  inline monomial &
  operator*=(const monomial &o) {
    coef *= o.coef;
    expo += o.expo;
    return *this;
  }
  /*!
   * \brief in-place division
   *
   * constant time
   */
  inline monomial &
  operator/=(const monomial &o) {
    assert(coef >= o.coef);
    coef /= o.coef;
    expo -= o.expo;
    return *this;
  }

  /*!
   * \brief in-place scalar multiplication
   *
   * constant time
   */
  inline monomial &
  operator*=(const K &o) {
    coef *= o;
    return *this;
  }
  /*!
   * \brief in-place scalar division
   *
   * constant time
   */
  inline monomial &
  operator/=(const K &o) {
    coef /= o;
    return *this;
  }

public:
  /*!
   * \brief Printing function
   *
   * constant time
   *
   * \param ios : an output stream
   */
  template<class S>
  inline void
  print(S &ios) const {
    ios << coef << '@' << expo;
  }

public:
  /*!
   * \brief Partial comparison function
   *
   * Here are just compared exponents,
   * see the addition operator.
   *
   * constant time
   *
   * \return the comparison value of exponents
   */
  static inline int
  compare(const monomial &a, const monomial &b) {
    return algebra::compare(a.expo, b.expo);
  }
  /*!
   * \brief Comparison function
   *
   * Here are compared exponents then coefficients,
   * (in this order for comparison compatibility).
   *
   * constant time
   *
   * \return the comparison value
   */
  static inline int
  deep_compare(const monomial &a, const monomial &b) {
    int cmpe = algebra::compare(a.expo, b.expo);
    if(cmpe != 0) return cmpe;
    return algebra::compare(a.coef, b.coef);
  }
};

/*
template<class K>
const monomial<K> monomial<K>::zero;

template<class K>
const monomial<K> monomial<K>::one (algebra::one<K>());
*/

}} // poly::sparse

#endif /* MONOMIAL_HXX_ */
