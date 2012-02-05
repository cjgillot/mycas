#include "analysis/pseries/repr.hpp"

using namespace analysis;

bool pseries::null() const
{
  return Bool_val( pseries_export_null( m_rep->m_value ) );
}

bool pseries::unit() const
{
  return Bool_val( pseries_export_unit( m_rep->m_value ) );
}
