/*
 * toom22.hpp
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef POLY_DENSE_MULTIPLY_TOOM22_HPP_
#define POLY_DENSE_MULTIPLY_TOOM22_HPP_

#define TOOM22_THRESHOLD 20

#include "stdlib.hpp"
#include "utils.hpp"

#include "algebra/print.hpp"

#include "polynomial/dense/multiply/common.hpp"

#include "polynomial/dense/multiply/basecase.hpp"
#include "polynomial/dense/multiply/split.hpp"

namespace poly {
namespace dense {
namespace multiply {

/*!
 * \struct toom22
 *
 * \brief 2-2 toom-cook (karatsuba) multiplication
 *
 * Given \f$(A, B) \in K[X]^2\f$, \f$ C = A B \f$
 * \f$ A = A_0 + X^p A_1 \f$ and \f$ B = B_0 + X^p B_1 \f$
 * Then \f$ C = C_0 + X^p C_1 + X^{2p} C_2 \f$ where :
 * -> \f$ C_0 = A_0 B_0 \f$
 * -> \f$ C_2 = A_1 B_1 \f$
 * -> \f$ C_1 = (A_0 - A_1) (B_0 - B_1) + C_0 + C_2 \f$
 */
template<class K, class I1, class I2, class I3>
struct toom22 {
  /*!
   * \brief algorithm function
   *
   * \param a,b : ranges
   * \param ret : an iterator
   */
  static inline void
  recursive(
    const I1 &ab, const I1 &ae
  , const I2 &bb, const I2 &be
  , I3 ret
  ) {
    int s1 = ae - ab,
        s2 = be - bb;

    // make sure that the second range is
    // ALWAYS smaller than the first one
    // -> ease asum and bsum
    int step = (1 + std::max(s1, s2)) >> 1;

    I1 a0, a1, a2; I2 b0, b1, b2;
    tuple::tie(a0, a1, a2) = split::split<2>(ab, ae, step);
    tuple::tie(b0, b1, b2) = split::split<2>(bb, be, step);

    typedef std::vector<K> VR;
    typedef typename VR::iterator VI;

    std::vector<K> rmid;
    {
      VR asum(a0, a1), bsum(b0, b1);

      // asum -= [a1, a2]
      detail::func_range(asum.begin(), a1, a2, functor::minus_eq<K>());

      // bsum -= [b1, b2]
      detail::func_range(bsum.begin(), b1, b2, functor::minus_eq<K>());

      rmid.resize(asum.size() + bsum.size() - 1);

      toom22<K, VI, VI, VI>::do_mul(
        asum.begin(), asum.end()
      , bsum.begin(), bsum.end()
      , rmid.begin()
      );
    }

    {
      std::vector<K> r00(2 * step - 1);

      toom22<K, I1, I2, VI>::do_mul(
        a0, a1
      , b0, b1
      , r00.begin()
      );

      // rmid += r00
      detail::func_range(rmid.begin(), r00.begin(), r00.end(), functor::plus_eq<K>());

      // ret += r00
      detail::func_range(ret         , r00.begin(), r00.end(), functor::plus_eq<K>());
    }

    {
      std::vector<K> r11(s1 + s2 - 1 - 2 * step);

      toom22<K, I1, I2, VI>::do_mul(
        a1, a2
      , b1, b2
      , r11.begin()
      );

      // rmid += r11
      detail::func_range(rmid.begin(), r11.begin(), r11.end(), functor::plus_eq<K>());

      // ret << 2*step += r11
      detail::func_range(ret + 2*step, r11.begin(), r11.end(), functor::plus_eq<K>());
    }

    // ret << step += rmid
    detail::func_range(ret+step, rmid.begin(), rmid.end(), functor::plus_eq<K>());
  }

  /*!
   * \brief main function
   *
   * \param a,b : ranges
   * \param ret : an iterator
   */
  static inline void
  do_mul(const I1 &ab, const I1 &ae, const I2 &bb, const I2 &be, const I3 &ret) {
    {
      int s1 = ae - ab, s2 = be - bb;
      if(( s1 < TOOM22_THRESHOLD )
       | ( s2 < TOOM22_THRESHOLD ))
      {
        naive<K, I1, I2, I3>::do_mul(ab, ae, bb, be, ret);
        return;
      }
    }

    recursive(ab, ae, bb, be, ret);
  }
};

}}} // poly::dense::multiply


#endif /* POLY_DENSE_MULTIPLY_TOOM22_HPP_ */
