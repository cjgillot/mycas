/*
 * expr.cpp
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "analysis/expr.hpp"
#include "analysis/numeric.hpp"

#include "analysis/sum.hpp"
#include "analysis/prod.hpp"
#include "analysis/power.hpp"

namespace analysis {

const unsigned expr::default_eval_depth = 10;

/*
expr::expr(const expr &o)
: m_impl(o.m_impl) {}
expr &expr::operator=(const expr &o) {
  m_impl = o.m_impl;
  return *this;
}

expr::expr(const basic* p)
: m_impl(p) {}

expr::~expr() {}

void expr::swap(expr &o)
{ m_impl.swap(o.m_impl); }
*/

// tests
bool expr::null() const {
  if( m_impl->null() ) {
    expr( number::zero() ).m_impl.swap(m_impl);
    return true;
  }
  return false;
}
bool expr::unit() const {
  if( m_impl->unit() ) {
    expr( number::one() ).m_impl.swap(m_impl);
    return true;
  }
  return false;
}

void expr::eval(unsigned lv) const {
  m_impl->eval(lv).m_impl.swap(m_impl);
}

bool expr::has(const symbol &s) const
{ return m_impl->has(s); }

expr expr::diff(const symbol &s, unsigned n) const
{ eval(); return m_impl->diff(s,n); }


#define PREPARE_SELF(klass)                     \
  const klass* ap =   get()->as_##klass()

#define PREPARE_OTHER(klass)                    \
  const klass* bp = b.get()->as_##klass()

// take care to cache the {b.get()} pointer _before_ cow'ing
#define PREPARE(klass)        \
  PREPARE_OTHER(klass);       \
  PREPARE_SELF (klass)

#define OPERATE_SELF(op)      \
  expr ret ( ap->op() );      \
  ret.eval();                 \
  return ret

#define OPERATE(op)           \
  expr ret ( ap->op(*bp) );   \
  ret.eval();                 \
  return ret

expr expr::operator+(const expr &b) const {
//   if(!b.get()) return *this;
//   if(!get())   return b;

  PREPARE(sum);
  OPERATE(add);
}
expr expr::operator-(const expr &b) const {
//   if(!b.get()) return *this;
//   if(!get())   return b.neg();

  PREPARE(sum);
  OPERATE(sub);
}

expr expr::operator*(const expr &b) const {
//   if(!b.get()) return b;
//   if(!get())   return *this;

  PREPARE(prod);
  OPERATE(mul);
}
expr expr::operator/(const expr &b) const {
//   assert(b.get());
//   if(!get())   return *this;

  PREPARE(prod);
  OPERATE(div);
}

expr expr::neg() const {
//   if(!get()) return *this;

  PREPARE_SELF(sum);
  OPERATE_SELF(neg);
}

expr expr::inv() const {
//   if(!get()) return *this;

  PREPARE_SELF(prod);
  OPERATE_SELF(inv);
}

expr expr::pow(const expr &o) const {
  expr ret ( power::from_be(get(), o.get()) );
  ret.eval();
  return ret;
}

int
expr::compare(const expr &a, const expr &b) {
  if(a.m_impl == b.m_impl)
    return 0;

  int c = basic::compare(*a.m_impl, *b.m_impl);
  if(c) return c;

  // here, a == b
  util::unify_ptr(a.m_impl, b.m_impl);
  return 0;
}

} // namespace analysis
