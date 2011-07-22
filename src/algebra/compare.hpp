/*
 * compare.hpp
 *
 *  Created on: 12 juin 2011
 *      Author: k1000
 */

#ifndef COMPARE_HPP_
#define COMPARE_HPP_

#include <boost/range.hpp>
#include <boost/range/iterator.hpp>

#include "operators.hpp"

namespace algebra {

using operators::compare;

template<class Range1, class Range2, class Cmp>
inline int
range_compare(const Range1 &a, const Range2 &b, Cmp f) {
  typename boost::range_iterator<const Range1>::type
    i1 = boost::begin(a),
    e1 = boost::end  (a);
  typename boost::range_iterator<const Range2>::type
    i2 = boost::begin(b),
    e2 = boost::end  (b);

  // base case
  for(; i1 != e1 && i2 != e2; ++i1, ++i2) {
    int cmp = f(*i1, *i2);
    if(cmp != 0) return cmp;
  }

  // at least one is empty
  if(i1 != e1) return 1;
  if(i2 != e2) return -1;

  // both are empty
  return 0;
}


}

#endif /* COMPARE_HPP_ */
