/*
 * basecase.hpp
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef POLY_DENSE_MULTIPLY_BASECASE_HPP_
#define POLY_DENSE_MULTIPLY_BASECASE_HPP_

#include "common.hpp"

namespace poly {
namespace dense {
namespace multiply {

/*!
 * \brief scalar multiplication
 *
 * Given \f$ A \in K[X]\f$, \f$ k \in K \f$,
 * \f$ Left = k A \f$ and \f$ Right = A k \f$
 * Then \f$ Left_i = k A_i \f$ and \f$ Right_i = A_i k \f$
 *
 */
template<class K, class I1, class I3>
struct scalar {
  /*!
   * \brief Right scalar multiplication
   *
   * [ret] becomes [ret + p * k]
   *
   * \param pb,pe : an input iterator pair
   * \param k : a non-null scalar
   * \param ret : an iterator
   */
  static inline void
  right(const I1 &pb, const I1 &pe, const K &k, I3 ret) {
    if(algebra::null(k))
      return;

    std::transform(
      pb, pe // input range
    , detail::make_func_applier(ret, functor::plus_eq<K>()) // output iterator
    , functor::multiplies_right<K>(k) // functor
    );
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
  left(const K &k, const I1 &pb, const I1 &pe, I3 ret) {
    if(algebra::null(k))
      return;

    std::transform(
      pb, pe // input range
    , detail::make_func_applier(ret, functor::plus_eq<K>()) // output iterator
    , functor::multiplies_left<K>(k) // functor
    );
  }
};

/*!
 * \brief Simple multiplication based on the convolution rule.
 *
 * Given \f$ (A, B) \in K[X]^2 \f$, \f$ C = A B \f$
 * Then \f$ C_k = \sum_{i+j=k} A_i B_j \f$
 */
template<class K, class I1, class I2, class I3>
struct naive {
  //! \brief Currification functor of \c scalar<>::left
  struct fe_lambda {
    const I2 &b, &e;
    I3 r;

    fe_lambda(const I2 &b_, const I2 &e_, const I3 &r_)
    : b(b_), e(e_), r(r_) {}

    fe_lambda(const fe_lambda &o)
    : b(o.b), e(o.e), r(o.r) {}

    fe_lambda&
    operator=(const fe_lambda &o)
    {
      b = o.b; e = o.e; r = o.r;
      return *this;
    }

    void operator()(const K &k)
    { scalar<K,I2,I3>::left(k, b, e, r); ++r; }
  };

  /*!
   * \param (ab,ae),(bb,be) : input iterator pairs
   * \param ret : output iterator
   */
  static inline void
  convolution(const I1 &ab, const I1 &ae, const I2 &bb, const I2 &be, const I3 &ret) {
    std::for_each(
      ab, ae // input range
    , fe_lambda(bb, be, ret) // functor
    );
  }

  /*!
   * \brief main function
   *
   * \param (ab,ae),(bb,be) : input iterator pairs
   * \param ret : output iterator
   */
  static inline void
  do_mul(const I1 &ab, const I1 &ae, const I2 &bb, const I2 &be, I3 ret) {
    {
      int s1 = ae - ab,
          s2 = be - bb;
      if(s1 == 0 || s2 == 0)
        return;
      if(s1 == 1) {
        scalar<K,I2,I3>::left (*ab, bb, be, ret);
        return;
      }
      if(s2 == 1) {
        scalar<K,I1,I3>::right(ab, ae, *bb, ret);
        return;
      }
    }

    convolution(ab, ae, bb, be, ret);
  }
};

}}} // namespace poly::dense::multiply

#endif /* POLY_DENSE_MULTIPLY_BASECASE_HPP_ */
