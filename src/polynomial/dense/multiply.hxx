/*
 * multiply.hxx
 *
 *  Created on: 9 juin 2011
 *      Author: k1000
 */

#ifndef MULTIPLY_HXX_
#define MULTIPLY_HXX_

namespace poly {
namespace dense {
namespace multiply {

// apply simple convolution rule
template<class K, class P>
struct naive {
  static inline void
  scalar(const P &p, const K &k, std::vector<K> &ret) {
    assert(! algebra::null(k));
    ret.resize(p.size());

    typename P::const_iterator
      it = p.begin(),
      end= p.end();
    typename std::vector<K>::iterator
      ir = ret.begin();

    for(; it != end; ++it, ++ir)
      *ir = *it * k;
  }
  static inline void
  do_mul(const P &a, const P &b, std::vector<K> &ret) {
    {
      int s1 = a.size(),
          s2 = b.size();
      if(s1 == 0 || s2 == 0) {
        ret.clear();
        return;
      }
      if(s1 == 1) {
        scalar(b, a.front(), ret);
        return;
      }
      if(s2 == 1) {
        scalar(a, b.front(), ret);
        return;
      }
      ret.resize(s1 + s2 - 1);
    }
    typename P::const_iterator
      i1 = a.begin(),
      e1 = a.end(),
      i2 = b.begin(),
      e2 = b.end();

    typename std::vector<K>::iterator
      ir0 = ret.begin(),
      ir = ir0;

    for(; i1 != e1; ++i1, ir = ++ir0)
    for(; i2 != e2; ++i2, ++ir) {
      *ir = *i1 * *i2;
    }
  }
};

}}} // poly::dense::multiply

#endif /* MULTIPLY_HXX_ */
