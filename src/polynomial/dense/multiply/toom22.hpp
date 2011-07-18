/*
 * toom22.hpp
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef POLY_DENSE_MULTIPLY_TOOM22_HXX_
#define POLY_DENSE_MULTIPLY_TOOM22_HXX_

#define TOOM22_THRESHOLD 20

#include "stdlib.hpp"
#include "utils.hpp"

#include "algebra/print.hpp"

#include "polynomial/dense/multiply/basecase.hpp"
#include "polynomial/dense/multiply/split.hpp"

namespace poly {
namespace dense {
namespace multiply {

namespace detail {

/*!
 * \fn void func_range(I1 i1, I2 i2, const I2 &e2, const F &f)
 *
 * \param i1 : an iterator
 * \param i2,e2 : an iterator pair
 * \param f ; a binary function
 *
 * \brief equivalent to :
 * \code
 *   for(; i2 != e2; ++i1, ++i2)
 *     f(*i1, *i2);
 * \endcode
 */
template<class I1, class I2, class F>
inline void
func_range(I1 i1, I2 i2, const I2 &e2, const F &f) {
  for(; i2 != e2; ++i1, ++i2)
    f(*i1, *i2);
}

} // namespace detail

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
 * -> \f$ C_1 = (A_0 + A_1) (B_0 + B_1) - C_0 - C_1 \f$
 */
template<class K, class R1, class R2, class I3>
struct toom22 {
  /*!
   * \brief algorithm function
   *
   * \param a,b : ranges
   * \param ret : an iterator
   */
  static inline void
  recursive(const R1 &a, const R2 &b, I3 ret) {
    typedef typename boost::range_iterator<const R1>::type It1;
    typedef typename boost::range_iterator<const R2>::type It2;

    int s1 = boost::size(a),
        s2 = boost::size(b);

    // make sure that the second range is
    // ALWAYS smaller than the first one
    // -> ease asum and bsum
    int step = (1 + std::max(s1, s2)) >> 1;

    It1 a0, a1, a2; It2 b0, b1, b2;
    tuple::tie(a0, a1, a2) = split::split<2, const R1>(a, step);
    tuple::tie(b0, b1, b2) = split::split<2, const R2>(b, step);

    typedef boost::iterator_range<It1> SR1;
    typedef boost::iterator_range<It2> SR2;

    typedef std::vector<K> VR;
    typedef typename std::vector<K>::iterator VI;

    std::vector<K> rmid;
    {
      std::vector<K>
        asum(a0, a1),
        bsum(b0, b1);

      detail::func_range(asum.begin(), a1, a2, functor::plus_eq<K>());
      detail::func_range(bsum.begin(), b1, b2, functor::plus_eq<K>());

      rmid.resize(asum.size() + bsum.size() - 1);

      toom22<K, VR, VR, VI>::do_mul(
        asum, bsum
      , rmid.begin()
      );
    }

    {
      std::vector<K> r00(2 * step - 1);

      toom22<K, SR1, SR2, VI>::do_mul(
        boost::make_iterator_range(a0, a1)
      , boost::make_iterator_range(b0, b1)
      , r00.begin()
      );

      detail::func_range(rmid.begin(), r00.begin(), r00.end(), functor::minus_eq<K>());
      detail::func_range(ret         , r00.begin(), r00.end(), functor::plus_eq<K>());
    }

    {
      std::vector<K> r11(s1 + s2 - 1 - 2 * step);

      toom22<K, SR1, SR2, VI>::do_mul(
        boost::make_iterator_range(a1, a2)
      , boost::make_iterator_range(b1, b2)
      , r11.begin()
      );

      detail::func_range(rmid.begin(), r11.begin(), r11.end(), functor::minus_eq<K>());
      detail::func_range(ret + 2*step, r11.begin(), r11.end(), functor::plus_eq<K>());
    }

    detail::func_range(ret+step, rmid.begin(), rmid.end(), functor::plus_eq<K>());
  }

  /*!
   * \brief main function
   *
   * \param a,b : ranges
   * \param ret : an iterator
   */
  static inline void
  do_mul(const R1 &a, const R2 &b, I3 ret) {
    {
      int s1 = boost::size(a),
          s2 = boost::size(b);
      if(s1 < TOOM22_THRESHOLD || s2 < TOOM22_THRESHOLD) {
        naive<K, R1, R2, I3>::do_mul(a,b, ret);
        return;
      }
    }

    recursive(a,b, ret);
  }
};

}}} // poly::dense::multiply


#endif /* POLY_DENSE_MULTIPLY_TOOM22_HXX_ */
