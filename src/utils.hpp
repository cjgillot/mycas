/*
 * utils.hpp
 *
 *  Created on: 10 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include<functional>

#include "util/functor.hpp"
#include "util/refcounted.hpp"
#include "util/ptr.hpp"
#include "util/move.hpp"

namespace util {

inline unsigned
pad_boolp(bool a, bool b) {
  a &= 1;  b &= 1; // this may not be needed
  a <<= 1; a |= b;
  return (a << 1 | b);
}

}

#endif /* UTILS_HPP_ */
