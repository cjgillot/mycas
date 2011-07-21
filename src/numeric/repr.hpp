#ifndef NUMERIC_REPR_HPP
#define NUMERIC_REPR_HPP

#include "number.hpp"

#include "util/refcounted.hpp"

#include<gmpfrxx.h>

namespace numeric {

struct number::repr_t
: private boost::noncopyable {

  MAKE_REFCOUNTED(repr_t);

public:
  template<class Int>
  repr_t(Int i, typename enable_if_int<Int>::type* = 0)
  : m_impl(mpz_class(i)) {}

  template<class Float>
  repr_t(Float f, typename enable_if_float<Float>::type* = 0)
  : m_impl(mpfr_class(f)) {}

  template<class T, class U>
  repr_t(const __gmp_expr<T,U> &e)
  : m_impl(__gmp_expr<T,T>(e)) {}

  ~repr_t() {}

public:
  bool null() const;
  bool unit() const;

public:
  // operators
  template<class OP>
  static repr_t* bin_op(const repr_t &, const repr_t &);

  template<class OP>
  static repr_t* una_op(const repr_t &);

  static const repr_t* pow(const repr_t &, const repr_t &);

public:
  static int
  compare(const repr_t &, const repr_t &);

  std::size_t
  hash() const;

  template<class S>
  void print(S &) const;

public:
  typedef boost::variant
  < mpz_class
  , mpq_class
  , mpfr_class
  > impl_t;

private:
  impl_t m_impl;
};

typedef number::repr_t repr_t;

}

#endif
