/*
 * utils.hpp
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

#ifndef ALGEBRA_UTILS_HPP_
#define ALGEBRA_UTILS_HPP_

#include "stdlib.hpp"
#include "operators.hpp"

namespace algebra {

template<class S>
inline S
zero()
{ return S::zero; }

template<class S>
inline S
one()
{ return S::one; }

template<class S>
inline bool
null(const S &o)
{ return o.null(); }

template<class S>
inline bool
unit(const S &o)
{ return o.unit(); }

template<class S>
inline S &
ineg(S &o)
{ return o.ineg(); }

template<class S>
inline S &
iinv(S &o)
{ return o.iinv(); }

} // namespace algebra

#endif /* ALGEBRA_UTILS_HPP_ */
