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

namespace analysis {

class numeric
: public basic
, public boost::field_operators1<numeric
, operators::printable<numeric
> > {

  algebra::real value;

  numeric();

public:
  numeric(const numeric &o)
  : basic(o), value(o.value) {}
  numeric &operator=(const numeric &o) {
    value = o.value;
    return *this;
  }
  void swap(numeric &o) {
    std::swap(value, o.value);
  }

public:
  numeric(double v)
  : value(v) {}
  ~numeric() {}

public:
  numeric* clone() const
  { return new numeric(value); }

public:
  bool null() const
  { return algebra::null(value); }
  bool unit() const
  { return algebra::unit(value); }

public:
  virtual bool is_numeric() const;
  virtual const add* as_add() const;
  virtual const mul* as_mul() const;

public:
  const numeric* plus()  const { return this; }
  const numeric* minus() const { return basic::cow<numeric>()->ineg(); }

  numeric* iadd(const numeric* o) {
    if(o) value += o->value;
    return this;
  }
  numeric* isub(const numeric* o) {
    if(o) value -= o->value;
    return this;
  }

  numeric* ineg() {
    value = -value;
    return this;
  }

  numeric* imul(const numeric* o) {
    if(o) value *= o->value;
    return this;
  }
  numeric* idiv(const numeric* o) {
    if(o) value /= o->value;
    return this;
  }

  numeric* iinv() {
    value = 1. / value;
    return this;
  }

  numeric* ipow(const numeric* o) {
    value = std::pow(value, o->value);
    return this;
  }

public:
  void print(std::basic_ostream<char> &os) const
  { os << value; }

  int compare_same_type(const basic&) const;
};


class number
: private ptr<numeric>
, public boost::field_operators1<number
, operators::printable<number
, operators::ordered<number
> > > {

  typedef ptr<numeric> super;

public:
  number(const number &o)
  : super(o) {}
  number &operator=(const number &o) {
    super::operator=(o);
    return *this;
  }
  void swap(number &o) {
    super::swap(o);
  }

  number(const numeric* n)
  : super(n) {}

public:
  number(double v)
  : super(new numeric(v)) {}
  ~number() {}

public:
  operator expr() const
  { return expr(super::get()); }

public:
  using super::null;
  using super::unit;
  using super::get;

  static const number zero;
  static const number one;

  const numeric* eval(unsigned) const {
    return get();
  }

public:
  friend number operator+(const number &a)
  { return a; }
  friend number operator-(number a)
  { return a.cow()->ineg(); }

  number &operator+=(const number &o) {
    if(!get()) return *this = o;
    cow()->iadd(o.get());
    return *this;
  }
  number &operator-=(const number &o) {
    if(!get()) return (*this = o).ineg();
    cow()->isub(o.get());
    return *this;
  }

  number &ineg() {
    if(!get()) return *this;
    cow()->ineg();
    return *this;
  }

  number &operator*=(const number &o) {
    if(!get()) return *this;
    cow()->imul(o.get());
    return *this;
  }
  number &operator/=(const number &o) {
    if(!get()) return *this;
    cow()->idiv(o.get());
    return *this;
  }

  number &iinv() {
    assert(get());
    cow()->iinv();
    return *this;
  }

  number &ipow(const number &o) {
    if(!get()) return *this;
    cow()->ipow(o.get());
    return *this;
  }
  number  pow (const number &o) {
    return number(*this).ipow(o);
  }

protected:
  virtual int
  do_compare(const numeric &o) const
  { return get()->compare_same_type(o); }

public:
  using super::print;

  static int
  compare(const number &a, const number &b)
  { return super::compare(a,b); }
};

}


#endif /* NUMERIC_HXX_ */
