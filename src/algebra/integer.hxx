/*
 * integer.hxx
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

#ifndef INTEGER_HXX_
#define INTEGER_HXX_

#include "algebra/utils.hxx"

namespace algebra {

typedef int integer;

template<>
inline integer
zero<integer>()
{ return 0; }

template<>
inline integer
one<integer>()
{ return 1; }

template<>
inline bool
null<integer>(const integer &o)
{ return o == 0; }

template<>
inline bool
unit<integer>(const integer &o)
{ return o == 1; }

template<>
inline int
compare<integer>(const integer &a, const integer &b)
{ return a-b; }

}

#endif /* INTEGER_HXX_ */
