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
{ return S::zero(); }

template<class S>
inline S
one()
{ return S::one(); }

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

/*!\brief Unit roots access
 *
 * This class is a template meant to be specialized
 * for each type \c K providing unit roots.
 *
 * \code
 * template<>
 * struct unit_root<K> {
 *   // we have unit roots
 *   enum { value = 1 };
 *
 *   // and provide them
 *   static K get(unsigned n)
 *   { return the first nth root }
 * };
 * \endcode
 *
 * \todo In practical cases (ie. FFT), you can assume \c n
 * is always a power of 2.
 */
template<class K>
struct unit_root {
  enum { value = 0 };
};

} // namespace algebra

#endif /* ALGEBRA_UTILS_HPP_ */
