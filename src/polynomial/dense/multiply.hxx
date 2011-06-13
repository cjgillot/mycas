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
namespace multiply {

#define IMPL toom22

template<class K, class R1, class R2, class I3>
struct mult
: IMPL<K, R1, R2, I3>
{ using IMPL<K, R1, R2, I3>::do_mul; };

#undef IMPL

}}} // poly::dense::multiply

#endif /* MULTIPLY_HXX_ */
