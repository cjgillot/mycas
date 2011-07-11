/*
 * numeric.cxx
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "numeric.hxx"

#include "add.hxx"
#include "mul.hxx"

namespace analysis {

const number number::zero(0.);
const number number::one(1.);

numeric::numeric(const numeric &o)
: basic(o), value(o.value) {}
numeric &numeric::operator=(const numeric &o) {
  value = o.value;
  return *this;
}
void numeric::swap(numeric &o) {
  std::swap(value, o.value);
}

numeric::numeric(double v)
: value(v) {}
numeric::~numeric() {}

numeric* numeric::clone() const
{ return new numeric(value); }

bool numeric::null() const
{ return algebra::null(value); }
bool numeric::unit() const
{ return algebra::unit(value); }

const numeric* numeric::plus()  const { return this; }
const numeric* numeric::minus() const { return clone()->ineg(); }

numeric* numeric::iadd(const numeric* o) {
  if(o) value += o->value;
  return this;
}
numeric* numeric::isub(const numeric* o) {
  if(o) value -= o->value;
  return this;
}

numeric* numeric::ineg() {
  value = -value;
  return this;
}

numeric* numeric::imul(const numeric* o) {
  if(o) value *= o->value;
  return this;
}
numeric* numeric::idiv(const numeric* o) {
  if(o) value /= o->value;
  return this;
}

numeric* numeric::iinv() {
  value = 1. / value;
  return this;
}

const numeric*
numeric::pow(const numeric* o) const {
  if(!o) return number::one.get();
  return new numeric(std::pow(value, o->value));
}

void numeric::print(std::basic_ostream<char> &os) const
{ os << value; }

bool
numeric::is_numeric() const
{ return true; }
const add*
numeric::as_add() const
{ return add::from_numeric(this); }
const mul*
numeric::as_mul() const
{ return mul::from_numeric(this); }

int numeric::compare_same_type(const basic &o) const {
  return algebra::compare(
    value
  , static_cast<const numeric&>(o).value
  );
}


number::number(const number &o)
: super(o) {}
number &number::operator=(const number &o) {
  super::operator=(o);
  return *this;
}
void number::swap(number &o) {
  super::swap(o);
}

number::number(const numeric* n)
: super(n) {}

number::number(double v)
: super(new numeric(v)) {}
number::~number() {}

number::operator expr() const
{ return expr(super::get()); }

number number::eval(unsigned) const
{ return *this; }

number &number::operator+=(const number &o) {
  if(!get()) return *this = o;
  cow()->iadd(o.get());
  return *this;
}
number &number::operator-=(const number &o) {
  if(!get()) return (*this = o).ineg();
  cow()->isub(o.get());
  return *this;
}

number &number::ineg() {
  if(!get()) return *this;
  cow()->ineg();
  return *this;
}

number &number::operator*=(const number &o) {
  if(!get()) return *this;
  cow()->imul(o.get());
  return *this;
}
number &number::operator/=(const number &o) {
  if(!get()) return *this;
  cow()->idiv(o.get());
  return *this;
}

number &number::iinv() {
  assert(get());
  cow()->iinv();
  return *this;
}

number number::pow(const number &o) const {
  if(!get())
    return number::zero;
  return get()->pow(o.get());
}

namespace {

struct comparator {
  inline int
  operator()(const numeric &a, const numeric &b)
  { return a.numeric::compare_same_type(b); }
};

}

int
number::compare(const number &a, const number &b)
{ return super::compare(a,b, comparator()); }

}
