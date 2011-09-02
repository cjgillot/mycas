#include "basic.hpp"
#include "sum.hpp"
#include "prod.hpp"
#include "power.hpp"

using namespace analysis;

const sum*   basic::as_sum()   const { return sum  ::from_basic(this); }
const prod*  basic::as_prod()  const { return prod ::from_basic(this); }
const power* basic::as_power() const { return power::from_basic(this); }

expr basic::pow( const expr &expo ) const
{
  return power::from_be( this, expo );
}


util::cmp_t
basic::compare(const basic &a, const basic &b) {
  // obvious case
  if(&a == &b) return 0;

  { // compare types
    util::cmp_t c = rtti::rtti_compare( RTTI_ID( a ), RTTI_ID( b ) );
    if( c ) return c;
  }

  { // we have same type
    // compare hash
    util::cmp_t c = util::compare( a.hash(), b.hash() );
    if( c ) return c;
  }

  // same type and hash collision

  // reaching this point should be
  // as rare as possible

  // deep compare
  return a.compare_same_type(b);
}
