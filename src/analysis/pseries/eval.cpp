#include "analysis/pseries/repr.hpp"

#include "analysis/pseries/iterator.hpp"

using namespace analysis;
using namespace pseries_detail;

expr pseries::eval(unsigned lv) const
{
  iterator it ( m_rep->m_value );

  if( lv == 0 )
    return this;

  --lv;
  for( ; it.forced() && !it.empty(); ++it )
    it->eval(lv);

  return basic::eval( ++lv );
}
