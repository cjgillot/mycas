#ifndef NUMERIC_REPR_HPP
#define NUMERIC_REPR_HPP

#include<gmpfrxx.h>
#include<boost/variant.hpp>

#include "util/refcounted.hpp"
#include "numeric/real/realfwd.hpp"

namespace numeric {

struct real_t
: private boost::noncopyable {

  MAKE_REFCOUNTED(real_t);

public:

  template<class Int>
  explicit
  real_t(Int i, typename enable_if_int<Int>::type* = 0)
  : m_impl(mpz_class(i)) {}

  template<class Float>
  explicit
  real_t(Float f, typename enable_if_float<Float>::type* = 0)
  : m_impl(mpfr_class(f)) {}

  template<class T, class U>
  real_t(const __gmp_expr<T,U> &e)
  : m_impl(__gmp_expr<T,T>(e)) {}

  ~real_t() {}

public:
  static const real_t* zero();
  static const real_t* one();

  bool null() const;
  bool unit() const;

public:
  // operators
  template<class OP>
  static real_t* bin_op(const real_t &, const real_t &);

  template<class OP>
  static real_t* una_op(const real_t &);

  static const real_t* pow(const real_t &, const real_t &);

public:
  static int
  compare(const real_t &, const real_t &);

  std::size_t
  hash() const;

  void print(std::ostream &) const;

public:
  typedef boost::variant
  < mpz_class
  , mpq_class
  , mpfr_class
  > impl_t;

private:
  impl_t m_impl;
};

}

#endif
