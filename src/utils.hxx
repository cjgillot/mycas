/*
 * utils.hxx
 *
 *  Created on: 10 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_HXX_
#define UTILS_HXX_

#include<functional>

#include "util/functor.hxx"
#include "util/refcounted.hxx"
#include "util/ptr.hxx"
#include "util/move.hxx"

namespace util {

inline unsigned
pad_boolp(bool a, bool b) {
  a &= 1;  b &= 1; // this may not be needed
  a <<= 1; a |= b;
  return (a << 1 | b);
}

}

#endif /* UTILS_HXX_ */
