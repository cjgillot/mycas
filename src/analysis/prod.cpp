/*
 * prod.cpp
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "analysis/sum.hpp"
#include "analysis/prod.hpp"

#include "expairseq.ipp"

namespace analysis {

struct prod::ep {
  static number add(const number &c1, const number &c2)
  { return c1 * c2; }

  static number sub(const number &c1, const number &c2)
  { return c1 / c2; }

  static number neg(const number &c1)
  { return c1.inv(); }
};

//************* prod class implementation ************//
prod::prod()
: super(number::one) {}

prod::prod(const prod &o)
: super(o) {}

void prod::swap(prod &o) {
  super::swap(o);
}

prod::~prod() {}

prod::prod(const number &n)
: super(n) {}

prod::prod(const number &n, const epair &p)
: super(n, p) {}

// operation constructors
template<class Tag>
prod::prod(const prod &a, const prod &b, Tag)
: super(a,b, Tag()) {}
prod::prod(const prod &a, neg_t)
: super(a, neg_t()) {}


// operations
prod* prod::mul(const prod &o) const
{ return new prod(*this, o, add_t()); }
prod* prod::div(const prod &o) const
{ return new prod(*this, o, sub_t()); }
prod* prod::inv() const
{ return new prod(*this, neg_t()); }

// test
bool prod::null() const {
  return super::coef().null();
}

bool prod::unit() const {
  return super::coef().unit()
      && super::is_empty();
}

// static members
prod* prod::from_1basic(const basic* a) {
  boost::intrusive_ptr<const power> pa = a->as_power();
  return new prod(number::one, pa.get()); // TODO
}
prod* prod::from_numeric(const numeric* n) {
  return new prod(n);
}

// eval
expr prod::eval(unsigned lv) const {
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
    expr b ( super::mono() );
    b.eval(lv);

    // (* 1 x) -> x
    if(c.unit())
      return b;

    // expand the addition TODO
    // (* c (+ c' a...)) -> (+ cc' ca...)
//     if(b.is_a<sum>()) {
//       sum* a = b.as_a<sum>()->cow<sum>();
//       a->imul(super::coef());
//       return expr(a);
//     }
  }

  return basic::eval(++lv);
}

}
