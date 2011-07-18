/*
 * operations.hpp
 *
 *  Created on: 24 mai 2011
 *      Author: c.gillot
 */

#ifndef OPERATIONS_HXX_
#define OPERATIONS_HXX_

namespace algebra {

template<class T>
inline T &
iadd(T &a, const T &b) {
  return a += b;
}

template<class T>
inline T &
isub(T &a, const T &b) {
  return a -= b;
}

template<class T>
inline T &
ineg(T &a) {
  return a.ineg();
}

template<class T>
inline T &
imul(T &a, const T &b) {
  return a *= b;
}

template<class T>
inline T &
idiv(T &a, const T &b) {
  return a /= b;
}

}

#endif /* OPERATIONS_HXX_ */
