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
  inline
  ptr()
  : p(0) {}
  inline
  ptr(T* p)
  : p(p) {}

  inline
  ptr(const ptr &o)
  : p(o.p) {}
  inline ptr &
  operator=(const ptr &o) {
    p=o.p;
    return *this;
  }

  inline
  ~ptr() {}

  inline void
  swap(ptr &o) {
    std::swap(p, o.p);
  }

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
