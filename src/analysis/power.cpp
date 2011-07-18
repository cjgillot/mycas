/*
 * power.cpp
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "utils.hpp"

#include "power.hpp"

#include "expair.ipp"

namespace analysis {

// ************** power traits implementation **********//

// expair::null testing
bool power_traits::ep::null_pair
  (const coef_type &e, const rest_type &r)
{ return e.null() || r.unit(); }

// expair::compare and expair::deep_compare
int power_traits::ep::compare_coef
  (const coef_type &e1, const coef_type &e2)
{ return expr::compare(e1, e2); }

int power_traits::ep::compare_rest
  (const rest_type &e1, const rest_type &e2)
{ return expr::compare(e1, e2); }

// all operations :
void power_traits::ep::add_coef
  (coef_type &e1, const coef_type &e2)
{ e1 += e2; }

void power_traits::ep::sub_coef
  (coef_type &e1, const coef_type &e2)
{ e1 -= e2; }

void power_traits::ep::mul_coef
  (coef_type &e1, const coef_type &e2)
{ e1 *= e2; }

void power_traits::ep::div_coef
  (coef_type &e1, const coef_type &e2)
{ e1 /= e2; }

void power_traits::ep::neg_coef
  (coef_type &e1)
{ e1.ineg(); }

// expair::print
void power_traits::ep::print_pair
  (std::basic_ostream<char> &os
  , const coef_type &e, const rest_type &b
  )
{
  os << "(^ " << b << " " << e << ')';
}

// expair::*_hash
std::size_t power_traits::ep::hash_coef
  (const coef_type &c)
{
  return c.get_hash();
}
std::size_t power_traits::ep::hash_rest
  (const rest_type &r)
{
  return r.get_hash();
}

//********** power class implementation ***********//

// ctors
power::power(const power &o)
: basic(o), m_impl(o.m_impl) {}

power::power(const basic* b, const basic* e)
  // swapped {e} and {b}
  // because the expair is { coef=e; rest=b }
: m_impl(expr(e), expr(b)) {}

void
power::swap(power &o) {
  m_impl.swap(o.m_impl);
}

power::~power() {}

// tests
bool power::null() const {
  return m_impl.m_rest.null();
}
bool power::unit() const {
  return m_impl.null();
}

expr power::eval(unsigned lv) const {
  if(lv == 0)
    return basic::eval(lv);
  --lv;

  const expr &e = m_impl.m_coef, &b = m_impl.m_rest;
  e.eval(lv); b.eval(lv);

  bool
    en = e.null(),
    eu = e.unit(),
    bn = b.null(),
    bu = b.unit();

  if(en | bu)
    return number::one;

  if(eu)
    return b;

  // FIXME (^ 0 x)
  if(bn)
    return expr(0);

  // TODO :
  // (^ (^ a b) c) -> (^ a (* b c)) if(x > 0 && c real)

  if(e.is_a<numeric>()) {

    if(b.is_a<numeric>()) {
      const number
        basis = b.as_a<numeric>(),
        expon = e.as_a<numeric>();
      return basis.pow(expon);
    }

    // TODO :
    // other cases (see ginac)

  }

  return basic::eval(++lv);
}

// coercion
power* power::clone() const
{ return new power(*this); }

const power*
power::as_power() const
{ return this; }

// misc.
void power::print(std::basic_ostream<char> &os) const
{ m_impl.print(os); }

int power::compare_same_type(const basic &b0) const {
  return impl_t::deep_compare(
    m_impl
  , static_cast<const power&>(b0).m_impl
  );
}

// static
const power*
power::from_be(const basic* b, const basic* e)
{ return new power(b, e); }

const power*
power::from_1basic(const basic* b)
{ return from_be(b, number::one.get()); }

const power*
power::from_numeric(const numeric* n)
{ return from_1basic(n); }

}
