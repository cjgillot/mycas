#ifndef REAL_HPP
#define REAL_HPP

#include "operators.hpp"
#include "util/refcounted.hpp"

#include "numeric/real/realfwd.hpp"

#if 0

namespace numeric {

class real
: private boost::field_operators1<real
, boost::incrementable<real
, operators::printable<real
> > > {

public:
  real()
  : m_repr(make_repr(0)) {}

  template<typename T>
  real(T i, typename enable_if_flint<T>::type = 0)
  : m_repr(make_repr(i)) {}

  real(const real &o)
  : m_repr(o.m_repr) {}

  real &
  operator=(const real &o)
  {
    m_repr = o.m_repr;
    return *this;
  }

  void swap(real &o)
  { m_repr.swap(o.m_repr); }

  ~real() {}

  explicit real(const real_t* p)
  : m_repr(p) {}

public:
  static real zero;
  static real one;

public:
  /*!
   * \name Modifying operators
   * \{
   */
  real &operator++();
  real &operator--();

  real &operator+=(const real &);
  real &operator-=(const real &);

  real &operator*=(const real &);
  real &operator/=(const real &);

  real &ineg();
  real &iinv();
  /*! \} */

  /*!
   * \name Persistent operators
   * \{
   */
  real neg() const
  { return real(*this).ineg(); }
  real inv() const
  { return real(*this).iinv(); }

  real pow(const real &) const;
  /*! \} */

public:
  bool null() const;
  bool unit() const;

public:
  void print(std::basic_ostream<char> &) const;

  static int
  compare(const real &, const real &);

  std::size_t
  get_hash() const;

public:
  const real_t* get() const
  { return m_repr.get(); }

private:
  boost::intrusive_ptr<const real_t> m_repr;
};

}

#else

#include<gmpfrxx.h>

namespace numeric {

typedef mpz_class integer;
typedef mpq_class rational;
typedef mpfr_class real;

}

#include "real/pow.hpp"

#endif

#endif
