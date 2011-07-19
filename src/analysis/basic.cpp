/*
 * basic.cpp
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "utils.hpp"

#include "basic.hpp"
#include "sum.hpp"
#include "prod.hpp"
#include "power.hpp"

namespace analysis {

basic::basic()
: m_flags(0), m_hash(0) {}
basic::basic(const basic &o)
: m_flags(o.m_flags), m_hash(o.m_hash) {}
basic::~basic() {}

void basic::swap(basic &o) {
  std::swap(m_hash, o.m_hash);
  std::swap(m_flags, o.m_flags);
}

bool
basic::null() const {
  return false;
}
bool
basic::unit() const {
  return false;
}

bool
basic::is_numeric() const {
  return false;
}
const sum*
basic::as_sum() const {
  return sum::from_1basic(this);
}
const prod*
basic::as_prod() const {
  return prod::from_1basic(this);
}
const power*
basic::as_power() const {
  return power::from_1basic(this);
}

expr
basic::eval(unsigned) const
{ return expr(this); }


int
basic::compare(const basic &a, const basic &b) {
  if(&a == &b) return 0;

  {
    std::size_t hash_a = a.get_hash(), hash_b = b.get_hash();

    int c = hash_a - hash_b;
    if(c) return c;
  }

  const std::type_info
    &ta = typeid(a)
  , &tb = typeid(b);

  // shall be optimized
  if(ta.before(tb)) return -1;
  if(tb.before(ta)) return 1;

  return a.compare_same_type(b);

/*
  switch(util::pad_boolp(a.is_numeric(), b.is_numeric())) {
  case 1: // b is numeric, a isn't
    return 1;
  case 2: // a is numeric, b isn't
    return -1;

  case 0: // both are non-numeric
    if(ta.before(tb))
      return -1;
    if(tb.before(ta))
      return 1;

  // fallback :
  case 3: // both have same type
    return a.compare_same_type(b);

  default:
    assert(false);
    return *reinterpret_cast<int*>(0);
  }
*/
}

}
