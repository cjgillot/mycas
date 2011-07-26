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
: super(number::one()) {}

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
  return new prod(number::one(), pa.get()); // TODO
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
    return number::zero();

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
    if(b.is_a<sum>()) {
      const sum* a = b.as_a<sum>();
      return expr( a->mul( c ) );
    }
  }

  return basic::eval(++lv);
}


expr prod::diff(const symbol &s, unsigned n) const
{
  if( n == 0 )
    return expr(this);

  //! now, n >= 1
  const number &c = super::coef();

  //! d^n(* c) -> 0
  if(super::is_empty())
    return number::zero();

  //! d^n(* 0 ...) -> 0
  if(c.null())
    return number::zero();

  //! d^n(* c x) -> (* c d^n(x))
  if(super::is_mono()) {
    expr b ( super::mono() );

    return expr(c) * b.diff(s, n);
  }

  if( n > 1 )
    return diff(s).diff(s, n-1);

  //! now, n == 1

  //! We use logarithmic differentiation :
  //!   \f[ e = \Pi_i p_i \f]
  //! \f[ ln(e) = \sum_i ln(p_i) --> dln(e) = de/e = \sum_i dln(p_i) \f]
  //! so, \f[ de = e * \sum_i dln(p_i) \f]
  //!
  //! Since \c p_i is a \c power, and that power differentiation
  //! needs logarithm, \c epair { aka. \c power::handle }
  //! provides it gently.

  expr ret;

  foreach(const epair &e, *this)
    if( e.has(s) )
      ret += e.diff_log(s);

  ret *= expr(this);
  return ret;
}


}
