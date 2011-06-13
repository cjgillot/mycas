/*
 * utils.hxx
 *
 *  Created on: 10 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_HXX_
#define UTILS_HXX_

#include<functional>
//#include "algebra/utils.hxx"
#include "algebra/compare.hxx"

namespace util {

using boost::range_detail::safe_bool;

class refcounted {
  mutable unsigned char refcount;

protected:
  inline
  refcounted()
  : refcount(0) {}
  inline
  refcounted(const refcounted &o)
  : refcount(0) {}
  inline refcounted &
  operator=(const refcounted &o) {
    refcount = 0;
    return *this;
  }

  ~refcounted() {}

public:
  friend inline void
  intrusive_ptr_add_ref(const refcounted* it) {
    assert(it != 0);
    ++it->refcount;
  }
  friend inline void
  intrusive_ptr_release(const refcounted* it) {
    assert(it != 0);
    if(--it->refcount == 0)
      delete it;
  }
};

}

namespace functor {

template<class T>
struct identity
: std::unary_function<T, T> {
  inline const T &
  operator()(const T &a) const
  { return a; }
};

using std::negate;

#define OP(name, op)                            \
  template<class T, class U=T, class R=T>       \
  struct name                                   \
  : std::binary_function<T, U, R> {             \
    inline R                                    \
    operator()(const T &a, const U &b) const    \
    { return a op b; }                          \
  }

#define OP_L(name, op)                          \
  template<class T, class U=T, class R=T>       \
  struct name                                   \
  : std::unary_function<U, R> {                 \
    T a;                                        \
    inline                                      \
    name(const T &a)                            \
    : a(a) {}                                   \
    inline R                                    \
    operator()(const U &b) const                \
    { return a op b; }                          \
  }

#define OP_R(name, op)                          \
  template<class T, class U=T, class R=T>       \
  struct name                                   \
  : std::unary_function<T, R> {                 \
    U b;                                        \
    inline                                      \
    name(const U &b)                            \
    : b(b) {}                                   \
    inline R                                    \
    operator()(const T &a) const                \
    { return a op b; }                          \
  }

#define OP_EQ(name, op)                         \
  template<class T, class U=T>                  \
  struct name                                   \
  : std::binary_function<T &, U, T &> {         \
    inline T &                                  \
    operator()(T &a, const U &b) const          \
    { return a op b; }                          \
  }

#define OP_BY(name, op)                         \
  template<class T, class U=T>                  \
  struct name                                   \
  : std::unary_function<T &, T &> {             \
    U b;                                        \
    inline                                      \
    name(const U &b)                            \
    : b(b) {}                                   \
    inline T &                                  \
    operator()(T &a) const                      \
    { return a op b; }                          \
  }

#define OP_TO(name, op)                         \
  template<class T, class U=T>                  \
  struct name                                   \
  : std::unary_function<U &, T &> {             \
    T &a;                                       \
    inline                                      \
    name(T &a)                                  \
    : a(a) {}                                   \
    inline T &                                  \
    operator()(const U &b)                      \
    { return a op b; }                          \
  }

#define OP_ALL(name, op)        \
  OP(name, op);                 \
  OP_L(name##_left, op);        \
  OP_R(name##_right,op);        \
  OP_EQ(name##_eq, op##=);      \
  OP_BY(name##_by, op##=);      \
  OP_TO(name##_to, op##=)

OP_ALL(plus, +);
OP_ALL(minus, -);
OP_ALL(multiplies, *);
OP_ALL(divides, /);

} // namespace functor

#include "operators.hxx"

namespace util {

template<class T>
struct ptr
: public operators::ordered<ptr<T>
, operators::printable<ptr<T> >
>  {
  T* p;

  ptr()
  : p(0) {}
  ptr(T* p)
  : p(p) {}
  ~ptr()
  {}

  inline T*
  get() const
  { return p; }
  inline T &
  operator*() const
  { return *p; }
  inline T*
  operator->() const
  { return p; }

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

#endif /* UTILS_HXX_ */
