/*
 * utils/ptr.hxx
 *
 *  Created on: 14 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_PTR_HXX_
#define UTILS_PTR_HXX_

#include "operators.hxx"
#include "algebra/compare.hxx"

namespace util {

template<class T>
class ptr
: public operators::ordered<ptr<T>
, operators::printable<ptr<T> >
> {
  T* p;

public:
  ptr()
  : p(0) {}
  ptr(T* p)
  : p(p) {}
  ~ptr() {}

public:
  inline T*
  get() const
  { return p; }
  inline T&
  operator*() const
  { return *p; }
  inline T*
  operator->() const
  { return p; }

public:
  template<class S>
  inline void
  print(S &ios) const {
    ios << *p;
  }
  static inline int
  compare(const ptr &a, const ptr &b) {
    return algebra::compare(*a.p, *b.p);
  }
};

} // namespace util

#endif /* UTILS_PTR_HXX_ */
