#include "analysis/pseries/repr.hpp"

#include "analysis/pseries/closure.hpp"

using namespace analysis;
using namespace pseries_detail;

static expr diff_closure( const expr &ex, void* data )
{
  return ex.diff( *(const symbol*)data );
}

expr pseries::differentiate(const symbol &s) const
{
  value ret;

  if( m_var.has( s ) )
    ret = pseries_export_diff( m_rep->m_value );

  else
  {
    map_closure_t closure =
      { &diff_closure, (void*)&s };

    ret = pseries_export_map( (value)&closure, m_rep->m_value );
  }

  analysis::ptr< pseries > serie = new pseries( m_var );
  serie->m_rep->m_value = ret;
  return serie.get();
}
