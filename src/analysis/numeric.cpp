/*
 * numeric.cpp
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "numeric.hpp"

#include "sum.hpp"
#include "prod.hpp"

namespace analysis {

const number number::zero(0.);
const number number::one(1.);

numeric::numeric(const numeric &o)
: basic(o), m_value(o.m_value) {}
numeric &numeric::operator=(const numeric &o) {
  m_value = o.m_value;
  return *this;
}
void numeric::swap(numeric &o) {
  std::swap(m_value, o.m_value);
}

numeric::numeric(double v)
: m_value(v) {}
numeric::~numeric() {}

numeric* numeric::clone() const
{ return new numeric(m_value); }

bool numeric::null() const
{ return algebra::null(m_value); }
bool numeric::unit() const
{ return algebra::unit(m_value); }

const numeric* numeric::plus()  const { return this; }
const numeric* numeric::minus() const { return clone()->ineg(); }

numeric* numeric::iadd(const numeric* o) {
  if(o) m_value += o->m_value;
  return this;
}
numeric* numeric::isub(const numeric* o) {
  if(o) m_value -= o->m_value;
  return this;
}

numeric* numeric::ineg() {
  m_value = -m_value;
  return this;
}

numeric* numeric::imul(const numeric* o) {
  if(o) m_value *= o->m_value;
  return this;
}
numeric* numeric::idiv(const numeric* o) {
  if(o) m_value /= o->m_value;
  return this;
}

numeric* numeric::iinv() {
  m_value = 1. / m_value;
  return this;
}

const numeric*
numeric::pow(const numeric* o) const {
  if(!o) return number::one.get();
  return new numeric(std::pow(m_value, o->m_value));
}

void numeric::print(std::basic_ostream<char> &os) const
{ os << m_value; }

bool
numeric::is_numeric() const
{ return true; }
const sum*
numeric::as_sum() const
{ return sum::from_numeric(this); }
const prod*
numeric::as_prod() const
{ return prod::from_numeric(this); }

int numeric::compare_same_type(const basic &o) const {
  return algebra::compare(
    m_value
  , static_cast<const numeric&>(o).m_value
  );
}

// ****** number ****** //
number::number(const number &o)
: m_impl(o.m_impl) {}
number &number::operator=(const number &o) {
  m_impl = o.m_impl;
  return *this;
}
void number::swap(number &o)
{ m_impl.swap(o.m_impl); }


number::number(const numeric* n)
: m_impl(n) {}

number::number(double v)
: m_impl(new numeric(v)) {}
number::~number() {}

number::operator expr() const
{ return expr(get()); }

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
{ return impl_t::compare(a.m_impl, b.m_impl, comparator()); }

}
