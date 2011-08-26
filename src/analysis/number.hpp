#ifndef NUMBER_HPP
#define NUMBER_HPP

#include "analysis/ptr.hpp"
#include "analysis/ptr.ipp"

#include "analysis/numeric.hpp"
#include "analysis/expr.hpp"

#include "operators.hpp"

namespace analysis {

class number
: public boost::field_operators1<number
, operators::printable<number
, operators::ordered<number
> > > {

  typedef ptr<numeric> impl_t;
  impl_t m_impl;

public:
  number(const number &);
  number &operator=(const number &);
  void swap(number &);

  number(const numeric*);

  number(double v);
  ~number();

public:
  operator expr() const;

private:
  numeric* cow()
  { return m_impl.cow(); }

public:
  inline bool
  null() const
  { return m_impl.null(); }
  inline bool
  unit() const
  { return m_impl.unit(); }

  inline const numeric*
  get() const
  { return m_impl.get(); }

  static const number &zero()
  {
    static const number value ( 0. );
    return value;
  }
  static const number &one()
  {
    static const number value ( 1. );
    return value;
  }

  number eval(unsigned) const;

public:
  friend number operator+(const number &a)
  { return a; }
  friend number operator-(number a)
  { return a.ineg(); }

  number &operator+=(const number &);
  number &operator-=(const number &);

  number &ineg();
  number   neg() const
  { return number(*this).ineg(); }

  number &operator*=(const number &);
  number &operator/=(const number &);

  number &iinv();
  number   inv() const
  { return number(*this).iinv(); }

  number  pow (const number &) const;

public:
  inline void
  print(std::basic_ostream<char> &os) const
  { m_impl.print(os); }

  static util::cmp_t
  compare(const number &, const number &);

  std::size_t hash() const
  { return m_impl.get()->numeric::hash(); }
};


}

#endif
