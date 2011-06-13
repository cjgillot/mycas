/*
 * toom22.hxx
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef POLY_DENSE_MULTIPLY_TOOM22_HXX_
#define POLY_DENSE_MULTIPLY_TOOM22_HXX_

#define TOOM22_THRESHOLD 10

#include "stdlib.hxx"
#include "utils.hxx"

#include "algebra/print.hxx"

#include "polynomial/dense/multiply/basecase.hxx"
#include "polynomial/dense/multiply/split.hxx"

namespace poly {
namespace dense {
namespace multiply {

template<class I1, class I2, class F>
inline void
func_range(I1 i1, I2 i2, const I2 &e2, const F &f) {
  for(; i2 != e2; ++i1, ++i2)
    f(*i1, *i2);
}

// 2-2 toom-cook (karatsuba) multiplication
template<class K, class R1, class R2, class I3>
struct toom22 {
  static inline void
  recursive(const R1 &a, const R2 &b, I3 ret) {
    typedef typename boost::range_iterator<const R1>::type It1;
    typedef typename boost::range_iterator<const R2>::type It2;

    int s1 = boost::size(a),
        s2 = boost::size(b);

    int sz = std::max(s1, s2);

    int step = sz / 2;

    It1 a0, a1, a2; It2 b0, b1, b2;
    tuple::tie(a0, a1, a2) = split<2, const R1>(a, step);
    tuple::tie(b0, b1, b2) = split<2, const R2>(b, step);

    typedef boost::iterator_range<It1> SR1;
    typedef boost::iterator_range<It2> SR2;

    toom22<K, SR1, SR2, I3>::do_mul(
      boost::make_iterator_range(a0, a1)
    , boost::make_iterator_range(b0, b1)
    , ret
    );
    I3 r00_b = ret, r00_e = ret + 2*step;

    toom22<K, SR1, SR2, I3>::do_mul(
      boost::make_iterator_range(a1, a2)
    , boost::make_iterator_range(b1, b2)
    , ret + 2*step
    );
    I3 r11_b = ret + 2*step, r11_e = ret + (s1 + s2 - 1);

    std::vector<K> rmid;
    {
      std::vector<K> asum(a1, a2);
      func_range(asum.begin(), a0, a1, functor::plus_eq<K>());

      std::vector<K> bsum(b1, b2);
      func_range(bsum.begin(), b0, b1, functor::plus_eq<K>());

      rmid.resize(asum.size() + bsum.size() - 1);

      toom22<K
      , std::vector<K>, std::vector<K>
      , typename std::vector<K>::iterator
      >::do_mul(asum, bsum, rmid.begin());

      func_range(rmid.begin(), r00_b, r00_e, functor::minus_eq<K>());
      func_range(rmid.begin(), r11_b, r11_e, functor::minus_eq<K>());
    }

    func_range(ret+step, rmid.begin(), rmid.end(), functor::plus_eq<K>());
  }

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
