/*
 * basecase.hpp
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef POLY_DENSE_MULTIPLY_BASECASE_HXX_
#define POLY_DENSE_MULTIPLY_BASECASE_HXX_

#include "stdlib.hpp"
#include "algebra/utils.hpp"

namespace poly {
namespace dense {
namespace multiply {

/*!
 * \struct scalar
 *
 * \brief scalar multiplication
 *
 * Given \f$ A \in K[X]\f$, \f$ k \in K\f$,
 * \f$ Left = k A\f$ and \f$ Right = A k \f$
 * Then \f$ Left[i] = k A[i]\f$ and \f$ Right[i] = A[i] k\f$
 *
 */
template<class K, class R1, class I3>
struct scalar {
  /*!
   * \brief Right scalar multiplication
   *
   * [ret] becomes [ret + p * k]
   *
   * \param p : a range
   * \param k : a non-null scalar
   * \param ret : an iterator
   */
  static inline void
  right(const R1 &p, const K &k, I3 ret) {
    assert(! algebra::null(k));

    typename boost::range_iterator<const R1>::type
      it = boost::begin(p),
      end= boost::end(p);

    for(; it != end; ++it, ++ret)
      *ret += *it * k;
  }

  /*!
   * \brief Left scalar multiplication
   *
   * [ret] becomes [ret + k * p]
   *
   * \param k : a non-null scalar
   * \param p : a range
   * \param ret : an iterator
   */
  static inline void
  left(const K &k, const R1 &p, I3 ret) {
    assert(! algebra::null(k));

    typename boost::range_iterator<const R1>::type
      it = boost::begin(p),
      end= boost::end(p);

    for(; it != end; ++it, ++ret)
      *ret += k * *it;
  }
};

/*!
 * \struct naive
 *
 * \brief Simple multiplication based on the convolution rule.
 *
 * Given \f$(A, B) \in K[X]^2\f$, \f$ C = A B \f$
 * Then \f$ C[k] = \sum_{i+j=k} A[i] B[j] \f$
 */
template<class K, class R1, class R2, class I3>
struct naive {
  /*!
   * \brief algorithm function
   *
   * \param a,b : ranges
   * \param ret : an iterator
   */
  static inline void
  convolution(const R1 &a, const R2 &b, I3 ret) {
    typename boost::range_iterator<const R1>::type
      i1 = boost::begin(a),
      e1 = boost::end(a);
    typename boost::range_iterator<const R2>::type
      i20= boost::begin(b),
      e2 = boost::end(b),
      i2 = i20;
    I3 ir = ret;

    for(        ; i1 != e1; ++i1, ir = ++ret)
    for(i2 = i20; i2 != e2; ++i2, ++ir)
      *ir += *i1 * *i2;
  }

  /*!
   * \brief main function
   *
   * \param a,b : ranges
   * \param ret : an iterator
   */
  static inline void
  do_mul(const R1 &a, const R2 &b, I3 &ret) {
    {
      int s1 = boost::size(a),
          s2 = boost::size(b);
      if(s1 == 0 || s2 == 0)
        return;
      if(s1 == 1) {
        scalar<K,R2,I3>::left (*boost::begin(a), b, ret);
        return;
      }
      if(s2 == 1) {
        scalar<K,R1,I3>::right(a, *boost::begin(b), ret);
        return;
      }
    }

    convolution(a,b,ret);
  }
};

}}} // namespace poly::dense::multiply

#endif /* POLY_DENSE_MULTIPLY_BASECASE_HXX_ */
