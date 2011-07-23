/*
 * multiply.hpp
 *
 *  Created on: 9 juin 2011
 *      Author: k1000
 */

#ifndef MULTIPLY_HPP_
#define MULTIPLY_HPP_

#include "stdlib.hpp"

#include "polynomial/dense/multiply/basecase.hpp"
#include "polynomial/dense/multiply/toom22.hpp"

namespace poly {
namespace dense {

/*!
 * \namespace poly::dense::multiply
 *
 * \brief Implementation of dense series multiplication algorithms
 */
namespace multiply {

#ifndef DENSE_MULT_IMPL
#define DENSE_MULT_IMPL toom22
#undef

/*!
 * \struct mult
 *
 * \brief A forwarding structure to the default algorithm.
 *
 * Multiplication algorithm specification :
 * the structure <tt>algo\<K, I1, I2, I3\></tt> must implement
 * a static \c do_mul(ab, ae, bb, be, r) member function.
 * <tt>(ab,ae)</tt> and <tt>(bb,be)</tt> are input iterator pairs,
 * \c r an output iterator.
 *
 * [r] becomes [r + a * b] \f$
 */
template<class K, class I1, class I2, class I3>
struct mult
: DENSE_MULT_IMPL<K, I1, I2, I3>
{ using DENSE_MULT_IMPL<K, I1, I2, I3>::do_mul; };

#undef DENSE_MULT_IMPL

/*!
 * \param a,b : two coefficient ranges representing polynomials,
 *      exponents in increasing order
 * \param ret : iterator to a range storing their product
 *      [ret += a * b]
 */
template<class R1, class R2, class I3>
inline void
prod(const R1 &a, const R2 &b, I3 ret) {
  typedef typename boost::range_value<R1>::type K;
  typedef typename boost::range_iterator<const R1>::type I1;
  typedef typename boost::range_iterator<const R2>::type I2;
  mult<K,I1,I2,I3>::do_mul(
    boost::begin(a),boost::end(a)
  , boost::begin(b),boost::end(b)
  , ret
  );
}

}}} // poly::dense::multiply

#endif /* MULTIPLY_HPP_ */
