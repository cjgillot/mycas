#include "analysis/pseries/pseries.hpp"
#include "analysis/pseries/repr.hpp"

#include "analysis/vectorseq.hpp"

using namespace analysis;

expr power::series(const symbol &s) const
{
  if( m_expo.is_numerical() )
  {
    number e = m_expo.as_a<numerical>()->get();
    if( e.fits_slong_p() )
    {
      long ne = e.get_slong();
      if( ne == -1 )
        return pseries::inv_series( m_base.get()->series(s).as_a<pseries>() );
    }
  }

  return basic::series(s);
}
