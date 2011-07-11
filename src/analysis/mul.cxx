/*
 * mul.cxx
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "analysis/add.hxx"
#include "analysis/mul.hxx"

#include "expairseq.ixx"

namespace analysis {

//************ mul traits implementation *************//

bool mul_traits::ep::null_coef
  (const coef_type &c)
{ return c.null(); }

void mul_traits::ep::add_coef
  (coef_type &c1, const coef_type &c2)
{ c1 += c2; }

void mul_traits::ep::sub_coef
  (coef_type &c1, const coef_type &c2)
{ c1 -= c2; }

void mul_traits::ep::mul_coef
  (coef_type &c1, const coef_type &c2)
{ c1 *= c2; }

void mul_traits::ep::div_coef
  (coef_type &c1, const coef_type &c2)
{ c1 /= c2; }

void mul_traits::ep::neg_coef
  (coef_type &c1)
{ c1.ineg(); }

void mul_traits::ep::inv_coef
  (coef_type &c1)
{ c1.iinv(); }

void mul_traits::ep::print_base
  (std::basic_ostream<char> &os)
{ os << "*"; }

//************* mul class implementation ************//
mul::mul()
: super(number::one) {}

mul::mul(const mul &o)
: super(o) {}

void mul::swap(mul &o) {
  super::swap(o);
}

mul::~mul() {}

mul::mul(const number &n)
: super(n) {}

mul::mul(const number &n, const epair &p)
: super(n, p) {}

// operations
mul* mul::imul(const mul& o) {
  super::iadd(o);
  return this;
}
mul* mul::idiv(const mul& o) {
  super::isub(o);
  return this;
}
mul* mul::iinv() {
  super::ineg();
  return this;
}

// test
bool mul::null() const {
  return super::coef().null();
}

bool mul::unit() const {
  return super::coef().unit()
      && super::is_empty();
}

// static members
mul* mul::from_1basic(const basic* a) {
  boost::intrusive_ptr<const power> pa = a->as_power();
  return new mul(number::one, *pa); // TODO
}
mul* mul::from_numeric(const numeric* n) {
  return new mul(n);
}

// eval
expr mul::eval(unsigned lv) const {
  const number &c = super::coef();

  // (* c) -> c
  if(super::is_empty())
    return c.eval(lv);

  // (* 0 ...) -> 0
  if(c.null())
    return expr(0);

  if(lv == 0)
    return expr(this);
  --lv;

  // (* c x) ?
  if(super::is_mono()) {
    const super::epair &m = super::mono();

    expr b (new power(m));
    b.eval(lv);

    // (* 1 x) -> x
    if(c.unit())
      return b;

    // expand the addition TODO
    // (* c (+ c' a...)) -> (+ cc' ca...)
//     if(b.is_a<add>()) {
//       add* a = b.as_a<add>()->cow<add>();
//       a->imul(super::coef());
//       return expr(a);
//     }
  }

  return basic::eval(++lv);
}

}
