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

// double
typedef double real;

template<>
inline real
zero<real>()
{ return 0.; }
template<>
inline real
one<real>()
{ return 0.; }

template<>
inline bool
null<real>(const real &o)
{ return o == 0.; }
template<>
inline bool
unit<real>(const real &o)
{ return o == 1.; }

inline int
compare(real a, real b)
{ return a-b; }
template<>
inline real &
ineg<real>(real &a)
{ return (a = -a); }

}

#endif /* INTEGER_HXX_ */
