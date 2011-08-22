#include "basic.hpp"
#include "sum.hpp"
#include "prod.hpp"
#include "power.hpp"

namespace analysis {

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


util::cmp_t
basic::compare(const basic &a, const basic &b) {
  // obvious case
  if(&a == &b) return 0;

  // compare types
  util::cmp_t c
    = rtti::rtti_compare( RTTI_ID( a ), RTTI_ID( b ) );
  if( c ) return c;

  // we have same type
  // compare hash
  c = util::compare( a.hash(), b.hash() );
  if(c) return c;

  // same type and hash collision

  // reaching this point should be
  // as rare as possible

  // deep compare
  return a.compare_same_type(b);
}

}
