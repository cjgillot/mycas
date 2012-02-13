#include "analysis/pseries/repr.hpp"
#include "analysis/pseries/iterator.hpp"

#include "util/foreach.hpp"

using namespace analysis;
using namespace pseries_detail;

expr pseries::eval(unsigned lv) const
{
  if( lv == 0 )
    return this;

  --lv;
  foreach( const expr &ex, *this )
    ex.eval(lv);

  return basic::eval( ++lv );
}
