/*
 * sum.hpp
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "analysis/sum.hpp"

#include "expairseq.ipp"

namespace analysis {

//************ sum traits implementation *************//

struct sum::ep {
  static number add(const number &c1, const number &c2)
  { return c1 + c2; }
  static number sub(const number &c1, const number &c2)
  { return c1 - c2; }
  static number neg(const number &c1)
  { return c1.neg(); }
  static number mul(const number &c1, const number &c2)
  { return c1 * c2; }
};

//************* sum class implementation ************//
//sum::sum()
//: super(number::zero) {}

sum::sum(const sum &o)
: super(o) {}

sum::~sum() {}

sum::sum(const number &n)
: super(n) {}

sum::sum(const number &n, const epair &p)
: super(n, p) {}

// operating ctors
template<class Tag>
sum::sum(const sum &a, const sum &b, Tag)
: super(a,b, Tag()) {}
sum::sum(const sum &a, neg_t)
: super(a, neg_t()) {}
sum::sum(const sum &a, const number &n, sca_t)
: super(a, n, sca_t()) {}

// operations
sum* sum::add(const sum &o) const
{ return new sum(*this, o, add_t()); }
sum* sum::sub(const sum &o) const
{ return new sum(*this, o, sub_t()); }
sum* sum::neg() const
{ return new sum(*this, neg_t()); }

sum* sum::mul(const number &n) const
{ return new sum(*this, n, sca_t()); }

// test
bool sum::null() const {
  return super::coef().null()
      && super::is_empty();
}

// static members
sum* sum::from_1basic(const basic* a) {
  boost::intrusive_ptr<const prod> pa = a->as_prod();
  return new sum(number::zero(), pa.get()); // TODO
}
sum* sum::from_numeric(const numeric* n) {
  return new sum(n);
}

// eval
expr sum::eval(unsigned lv) const {
  const number &c = super::coef();

  if(super::is_empty())
    return c;

  if(c.null() && super::is_mono()) {
    expr m ( super::mono() );
    m.eval(--lv);
    return m;
  }

  return basic::eval(lv);
}

// diff
expr sum::diff(const symbol &s, unsigned n) const {
  if( n == 0 )
    return expr(this);

  expr ret ( number::zero() );

  foreach( const epair &e, *this )
    if( e.ptr()->has(s) )
      ret += e.ptr()->diff( s, n );

  return ret;
}

}
