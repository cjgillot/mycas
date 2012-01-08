#include "analysis/power.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

using namespace analysis;

// ************** power handle implementation **********//

util::cmp_t
power::handle::compare(const handle &a, const handle &b)
{ return expr::compare(a.m_ptr->m_base, b.m_ptr->m_base); }

// all operations :
power::handle
power::handle::operator+(const handle &o) const {
  ASSERT(compare(*this, o) == 0);

  return new power(
    m_ptr->m_base
  , m_ptr->m_expo + o.m_ptr->m_expo
  );
}
power::handle
power::handle::operator-(const handle &o) const {
  ASSERT(compare(*this, o) == 0);

  return new power(
    m_ptr->m_base
  , m_ptr->m_expo - o.m_ptr->m_expo
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

// cdtor
power::power(const expr &b, const expr &e)
: m_base(b), m_expo(e) {}

// tests
bool power::null() const {
  return m_base.null();
}
bool power::unit() const {
  return m_expo.null() || m_base.unit();
}

expr power::eval(unsigned lv) const {
  if(lv == 0)
    return basic::eval( lv );
  --lv;

  bool
    en = m_expo.null(),
    eu = m_expo.unit(),
    bn = m_base.null(),
    bu = m_base.unit();

  if( en | bu )
    return 1l;

  if( eu )
    return m_base;

  // FIXME (^ 0 x)
  if( bn )
    return 0l;

  // TODO :
  // (^ (^ a b) c) -> (^ a (* b c)) if(x > 0 && c real)

  if(m_expo.is_numerical())
  {
    if(m_base.is_numerical())
    {
      const number &basis = m_base.as_a< numerical >()->get();
      const number &expon = m_expo.as_a< numerical >()->get();
      return basis.pow(expon);
    }

    // TODO :
    // other cases (see ginac)

  }

  return basic::eval( lv );
}

bool power::has(const symbol &s) const
{ return m_base.has(s) || m_expo.has(s); }

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

util::cmp_t
power::compare_same_type(const basic &o_) const {
  const power &o = static_cast<const power&>(o_);
  util::cmp_t c = expr::compare(m_base, o.m_base);
  return c ? c
  : expr::compare(m_expo, o.m_expo);
}

std::size_t power::hash() const {
  std::size_t seed = 0;
  boost::hash_combine(seed, m_base.get()->hash());
  boost::hash_combine(seed, m_expo.get()->hash());
  return seed;
}

// static
const power*
power::from_be(const expr &b, const expr &e)
{ return new power( b, e ); }

const power*
power::from_basic(const basic* b)
{ return from_be( b, 1l ); }
