#ifndef UTILS_FUNCTOR_HPP_
#define UTILS_FUNCTOR_HPP_

#include<functional>

/*!
 * \namespace functor
 *
 * \brief Miscellaneous utility functor objects
 *
 * Here are defined :
 * - identity
 * - plus/minus/multiplies/divides
 *   with their variants (rev, left, right,
 *   eq, by, to)
 */
namespace functor {

/*!
 * \struct identity
 *
 * \brief identity functor
 */
template<class T>
struct identity
: std::unary_function<const T&, const T&> {
  /*!
   * \brief Call operator
   */
  inline const T &
  operator()(const T &a) const
  { return a; }
};

using std::negate;

namespace detail {

template<class T>
struct call
{ typedef T type; }

template<class T>
struct call<T&>
{ typedef T type; }

template<class T>
struct call<const T&>
{ typedef const T &type; }

}

#define C( T ) typename call<T>::type

#define OP(name, op)                        \
  template<class T, class U=T, class R=T>   \
  struct name                               \
  : std::binary_function<T, U, R> {         \
    inline R                                \
    operator()( C(T) a, C(U) b) const       \
    { return a op b; }                      \
  }

#define OP_REV(name, op)                        \
  template<class T, class U=T, class R=T>       \
  struct name                                   \
  : std::binary_function<U, T, R> {             \
    inline R                                    \
    operator()( C(T) a, C(U) b) const           \
    { return a op b; }                          \
  }

#define OP_L(name, op)                          \
  template<class T, class U=T, class R=T>       \
  struct name                                   \
  : std::unary_function<U, R> {                 \
    T a;                                        \
    inline                                      \
    name( C(T) a_ ) const                       \
    : a(a_) {}                                  \
    inline R                                    \
    operator()( C(U) b ) const                  \
    { return a op b; }                          \
  }

#define OP_R(name, op)                          \
  template<class T, class U=T, class R=T>       \
  struct name                                   \
  : std::unary_function<T, R> {                 \
    U b;                                        \
    inline                                      \
    name( C(U) b_ )                             \
    : b(b_) {}                                  \
    inline R                                    \
    operator()( C(T) a ) const                  \
    { return a op b; }                          \
  }

#define OP_EQ(name, op)                         \
  template<class T, class U=T>                  \
  struct name                                   \
  : std::binary_function<T &, U, T &> {         \
    inline T &                                  \
    operator()(T &a, C(U) b) const              \
    { return a op b; }                          \
  }

#define OP_BY(name, op)                         \
  template<class T, class U=T>                  \
  struct name                                   \
  : std::unary_function<T &, T &> {             \
    U b;                                        \
    inline                                      \
    name(const U &b_)                           \
    : b(b_) {}                                  \
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
    name(T &a_)                                 \
    : a(a_) {}                                  \
    inline T &                                  \
    operator()( C(U) b)                         \
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

//! \brief {plus_*} functors around {+}
OP_ALL(plus, +);

//! \brief {minus_*} functors around {-}
OP_ALL(minus, -);

//! \brief {multiplies_*} functors around {*}
OP_ALL(multiplies, *);

//! \brief {divides_*} functors around {/}
OP_ALL(divides, /);

#undef OP_ALL
#undef OP_TO
#undef OP_BY
#undef OP_EQ
#undef OP_R
#undef OP_L
#undef OP_REV
#undef OP

} // namespace functor


#endif /* UTILS_FUNCTOR_HPP_ */
