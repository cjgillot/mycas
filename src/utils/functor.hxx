/*
 * functor.hxx
 *
 *  Created on: 14 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_FUNCTOR_HXX_
#define UTILS_FUNCTOR_HXX_

#include<functional>

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

#define OP_REV(name, op)                        \
  template<class T, class U=T, class R=T>       \
  struct name                                   \
  : std::binary_function<U, T, R> {             \
    inline R                                    \
    operator()(const U &b, const T &a) const    \
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
  OP_REV(name##_rev, op);       \
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


#endif /* UTILS_FUNCTOR_HXX_ */
