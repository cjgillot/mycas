#ifndef OPERATION_HPP
#define OPERATION_HPP

#include "real_t.hpp"

namespace numeric {

// all visitors and functions in this file must not return NULL

// una_op_v
namespace {

template<class OP>
struct una_op_v
: boost::static_visitor<real_t*> {

  template<class T>
  real_t*
  operator()(const T &a) const
  { return OP::template do_it<T>(a); }
};

#define DECL_UOP(name, op)  \
  struct name {             \
                            \
    template<class T>       \
    static real_t*          \
    do_it(const T &a) {     \
      return new real_t(    \
        op( a )             \
      );                    \
    }                       \
  }

}

template<class OP>
inline real_t*
real_t::una_op(const real_t &a)
{ return boost::apply_visitor( una_op_v<OP>() , a.m_impl ); }

// bin_op_v
namespace {

template<class OP>
struct bin_op_v
: boost::static_visitor<real_t*> {

  template<class T, class U>
  real_t*
  operator()(const T &a, const U &b) const
  { return OP::template do_it<T,U>(a, b); }
};

#define DECL_BOP(name, op)          \
  struct name {                     \
                                    \
    template<class T, class U>      \
    static real_t*                  \
    do_it(                          \
      const T &a                    \
    , const U &b                    \
    ) {                             \
      return new real_t(            \
        a op b                      \
      );                            \
    }                               \
  }

}

template<class OP>
inline real_t*
real_t::bin_op(const real_t &a, const real_t &b)
{ return boost::apply_visitor( bin_op_v<OP>() , a.m_impl, b.m_impl ); }

} // namespace numeric

#endif
