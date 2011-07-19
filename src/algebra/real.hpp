#ifndef REAL_HPP_
#define REAL_HPP_

#include "algebra/utils.hpp"

namespace algebra {

// long double
typedef double real;

int compare(real a, real b);

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
{ return compare(o, 0.l) == 0; }
template<>
inline bool
unit<real>(const real &o)
{ return compare(o, 1.l) == 0; }

template<>
inline real &
ineg<real>(real &a)
{ return (a = -a); }

}

#endif // REAL_HPP_
