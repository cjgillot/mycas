#include "analysis/pseries/repr.hpp"
#include "analysis/pseries/iterator.hpp"

#include "util/foreach.hpp"

using namespace analysis;
using namespace pseries_detail;

bool pseries::has(const symbol &s) const
{
  if( m_var.has( s ) )
    return true;

  foreach( const expr &ex, *this )
    if( ex.has( s ) )
      return true;

  return false;
}
