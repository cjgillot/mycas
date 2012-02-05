#include "analysis/pseries/repr.hpp"

#include "analysis/pseries/iterator.hpp"

using namespace analysis;
using namespace pseries_detail;

bool pseries::has(const symbol &s) const
{
  if( m_var.has( s ) )
    return true;

  iterator it ( m_rep->m_value );

  for( ; it.forced() && !it.empty(); ++it )
    if( it->has( s ) )
      return true;

  return false;
}
