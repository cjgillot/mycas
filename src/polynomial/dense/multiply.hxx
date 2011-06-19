/*
 * multiply.hxx
 *
 *  Created on: 9 juin 2011
 *      Author: k1000
 */

#ifndef MULTIPLY_HXX_
#define MULTIPLY_HXX_

#include "stdlib.hxx"

#include "polynomial/dense/multiply/basecase.hxx"
#include "polynomial/dense/multiply/toom22.hxx"

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
 * This one must add the result of [a * b] to the [r] iterator.
 */
template<class K, class R1, class R2, class I3>
struct mult
: IMPL<K, R1, R2, I3>
{ using IMPL<K, R1, R2, I3>::do_mul; };

#undef IMPL

template<class R1, class R2, class I3>
inline void
mul(const R1 &a, const R2 &b, I3 ret) {
  typedef typename boost::range_value<R1>::type K;
  mult<K,R1,R2,I3>::do_mul(a,b,ret);
}

}}} // poly::dense::multiply

#endif /* MULTIPLY_HXX_ */
