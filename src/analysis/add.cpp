/*
 * add.hpp
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "analysis/add.hpp"

#include "expairseq.ipp"

namespace analysis {

//************ add traits implementation *************//

void add_traits::ep::mul_coef
  (coef_type &c1, const coef_type &c2)
{ c1 += c2; }

void add_traits::ep::div_coef
  (coef_type &c1, const coef_type &c2)
{ c1 -= c2; }

void add_traits::ep::inv_coef
  (coef_type &c1)
{ c1.ineg(); }

void add_traits::ep::print_base
  (std::basic_ostream<char> &os)
{ os << "+"; }

//************* add class implementation ************//
//add::add()
//: super(number::zero) {}

add::add(const add &o)
: super(o) {}

add::~add() {}

add::add(const number &n)
: super(n) {}

add::add(const number &n, const epair &p)
: super(n, p) {}

// operations
add* add::iadd(const add& o) {
  super::iadd(o);
  return this;
}
add* add::isub(const add& o) {
  super::isub(o);
  return this;
}
add* add::ineg() {
  super::ineg();
  return this;
}

add* add::imul(const number &n) {
  super::imul(n);
  return this;
}

// test
bool add::null() const {
  return super::coef().null()
      && super::is_empty();
}

// static members
add* add::from_1basic(const basic* a) {
  boost::intrusive_ptr<const mul> pa = a->as_mul();
  return new add(number::zero, *pa); // TODO
}
add* add::from_numeric(const numeric* n) {
  return new add(n);
}

// eval
expr add::eval(unsigned lv) const {
  const number &c = super::coef();

  if(super::is_empty())
    return c;

  if(c.null() && super::is_mono()) {
    expr m (new mul(super::mono()));
    m.eval(--lv);
    return m;
  }

  return basic::eval(lv);
}

}
