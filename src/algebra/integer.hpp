/*
 * integer.hpp
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

#ifndef INTEGER_HPP_
#define INTEGER_HPP_

#include "algebra/utils.hpp"

namespace algebra {

// int
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

inline int
compare(integer a, integer b)
{ return a-b; }
template<>
inline integer &
ineg<integer>(integer &a)
{ return (a = -a); }

}

#endif /* INTEGER_HPP_ */