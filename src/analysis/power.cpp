/*
 * power.cpp
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "utils.hpp"

#include "analysis/power.hpp"

namespace analysis {

// ************** power handle implementation **********//

int power::handle::compare(const handle &a, const handle &b)
{ return expr::compare(a.m_ptr->m_base, b.m_ptr->m_base); }

// all operations :
power::handle
power::handle::operator+(const handle &o) const {
  assert(compare(*this, o) == 0);

  return new power(
    m_ptr->m_base
  , m_ptr->m_expo + m_ptr->m_expo
  );
}
power::handle
power::handle::operator-(const handle &o) const {
  assert(compare(*this, o) == 0);

  return new power(
    m_ptr->m_base
  , m_ptr->m_expo - m_ptr->m_expo
  );
}
power::handle
power::handle::operator-() const {
  return new power(
    m_ptr->m_base
  , - m_ptr->m_expo
  );
}

//********** power class implementation ***********//

// ctors
power::power(const power &o)
: basic(o), m_base(o.m_base), m_expo(o.m_expo) {}

power::power(const expr &b, const expr &e)
: m_base(b), m_expo(e) {}

void
power::swap(power &o) {
  basic::swap(o);
  m_base.swap(o.m_base);
  m_expo.swap(o.m_expo);
}

power::~power() {}

// tests
bool power::null() const {
  return m_base.null();
}
bool power::unit() const {
  return m_expo.null() | m_base.unit();
}

expr power::eval(unsigned lv) const {
  if(lv == 0)
    return basic::eval(lv);
  --lv;

  const expr &e = m_expo, &b = m_base;
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
void power::print(std::basic_ostream<char> &os) const {
  os << '(' << '^';
  m_base.print(os << ' ');
  m_expo.print(os << ' ');
  os << ')';
}

int power::compare_same_type(const basic &o_) const {
  const power &o = static_cast<const power&>(o_);
  int c = expr::compare(m_base, o.m_base);
  return c ? c
  : expr::compare(m_expo, o.m_expo);
}

std::size_t power::calc_hash() const {
  std::size_t seed = basic::calc_hash();
  boost::hash_combine(seed, m_base.get()->get_hash());
  boost::hash_combine(seed, m_expo.get()->get_hash());
  return seed;
}

// static
const power*
power::from_be(const basic* b, const basic* e)
{ return new power(expr(b), expr(e)); }

const power*
power::from_1basic(const basic* b)
{ return from_be(b, number::one.get()); }

const power*
power::from_numeric(const numeric* n)
{ return from_1basic(n); }

}
