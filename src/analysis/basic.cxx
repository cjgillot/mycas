/*
 * basic.cxx
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "utils.hxx"

#include "basic.hxx"
#include "add.hxx"
#include "mul.hxx"
#include "power.hxx"

namespace analysis {

basic::basic() {}
basic::basic(const basic &) {}
basic::~basic() {}

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
const add*
basic::as_add() const {
  return add::from_1basic(this);
}
const mul*
basic::as_mul() const {
  return mul::from_1basic(this);
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
