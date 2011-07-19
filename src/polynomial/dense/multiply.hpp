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
 * \brief Contains implementation of dense series
 *      multiplication algorithms
 */
namespace multiply {

#define IMPL toom22

/*!
 * \struct mult
 *
 * \brief A forwarding structure to the default
 *      dense multiplication algorithm.
 *
 * Multiplication algorithm specification :
 * the structure algo<K, R1, R2, I3> must implement
 * a static do_mul(const R1 &a, const R2 &b, I3 r) member function.
 * [a] and [b] are RA ranges, [r] and assignable iterator.
 * This one must sum the result of [a * b] to the [r] iterator.
 */
template<class K, class R1, class R2, class I3>
struct mult
: IMPL<K, R1, R2, I3>
{ using IMPL<K, R1, R2, I3>::do_mul; };

#undef IMPL

/*!
 * \fn void prod(const R1 &a, const R2 &b, I3 ret)
 * \param a,b : two coefficient ranges representing polynomials,
 *      exponents in increasing order
 * \param ret : iterator to a range storing their product
 *      [ret += a * b]
 */
template<class R1, class R2, class I3>
inline void
prod(const R1 &a, const R2 &b, I3 ret) {
  typedef typename boost::range_value<R1>::type K;
  mult<K,R1,R2,I3>::do_mul(a,b,ret);
}

}}} // poly::dense::multiply

#endif /* MULTIPLY_HPP_ */
