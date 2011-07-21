#ifndef OPERATION_HPP
#define OPERATION_HPP

#include "repr.hpp"

namespace numeric {

// all visitors and functions in this file must not return NULL

// una_op_v
namespace {

template<class OP>
struct una_op_v
: boost::static_visitor<repr_t*> {

  template<class T>
  repr_t*
  operator()(const T &a) const
  { return OP::template do_it<T>(a); }
};

#define DECL_UOP(name, op)  \
  struct name {             \
                            \
    template<class T>       \
    static repr_t*          \
    do_it(const T &a) {     \
      return new repr_t(    \
        op( a )             \
      );                    \
    }                       \
  }

}

template<class OP>
inline repr_t*
repr_t::una_op(const repr_t &a)
{ return boost::apply_visitor( una_op_v<OP>() , a.m_impl ); }

// bin_op_v
namespace {

template<class OP>
struct bin_op_v
: boost::static_visitor<repr_t*> {

  template<class T, class U>
  repr_t*
  operator()(const T &a, const U &b) const
  { return OP::template do_it<T,U>(a, b); }
};

#define DECL_BOP(name, op)          \
  struct name {                     \
                                    \
    template<class T, class U>      \
    static repr_t*                  \
    do_it(                          \
      const T &a                    \
    , const U &b                    \
    ) {                             \
      return new repr_t(            \
        a op b                      \
      );                            \
    }                               \
  }

}

template<class OP>
inline repr_t*
repr_t::bin_op(const repr_t &a, const repr_t &b)
{ return boost::apply_visitor( bin_op_v<OP>() , a.m_impl, b.m_impl ); }

} // namespace numeric

#endif
