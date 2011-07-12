/*
 * numeric.hxx
 *
 *  Created on: 20 juin 2011
 *      Author: k1000
 */

#ifndef NUMERIC_HXX_
#define NUMERIC_HXX_

#include "algebra/real.hxx"

#include "operators.hxx"

#include "analysis/basic.hxx"
#include "analysis/expr.hxx"

#include "analysis/ptr.ixx"

namespace analysis {

class numeric
: public basic
, public boost::field_operators1<numeric
, operators::printable<numeric
> > {

  algebra::real m_value;

  numeric();

public:
  numeric(const numeric &);
  numeric &operator=(const numeric &);
  void swap(numeric &);

public:
  numeric(double v);
  ~numeric();

public:
  numeric* clone() const;

public:
  bool null() const;
  bool unit() const;

public:
  virtual bool is_numeric() const;
  virtual const add* as_add() const;
  virtual const mul* as_mul() const;

public:
  const numeric* plus()  const;
  const numeric* minus() const;

  numeric* iadd(const numeric*);
  numeric* isub(const numeric*);

  numeric* ineg();

  numeric* imul(const numeric*);
  numeric* idiv(const numeric*);

  numeric* iinv();

  const numeric* pow(const numeric* o) const;

public:
  void print(std::basic_ostream<char> &os) const;
  int compare_same_type(const basic&) const;

private:
  std::size_t calc_hash() const
  { return boost::hash<algebra::real>()(m_value); }
};


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

  static const number zero;
  static const number one;

  number eval(unsigned) const;

public:
  friend number operator+(const number &a)
  { return a; }
  friend number operator-(number a)
  { return a.ineg(); }

  number &operator+=(const number &);
  number &operator-=(const number &);

  number &ineg();

  number &operator*=(const number &);
  number &operator/=(const number &);

  number &iinv();

  number  pow (const number &) const;

public:
  inline void
  print(std::basic_ostream<char> &os) const
  { m_impl.print(os); }

  static int
  compare(const number &, const number &);

  std::size_t
  get_hash() const
  { return m_impl.get()->get_hash(); }
};

}


#endif /* NUMERIC_HXX_ */
