#include "analysis/pseries/repr.hpp"

#include "analysis/pseries/iterator.hpp"

using namespace analysis;
using namespace pseries_detail;

util::cmp_t pseries::compare_same_type(const basic &o_) const
{
  const pseries &o = static_cast<const pseries&>( o_ );

  util::cmp_t c = symbol::compare( m_var, o.m_var );
  if( c ) return c;

  if( m_rep == o.m_rep )
    return 0;

  c = pseries_export_compare( m_rep->m_value, o.m_rep->m_value );

  if( c == 0 )
    util::unify_ptr(
      const_cast<boost::intrusive_ptr<repr>&>(  m_rep)
    , const_cast<boost::intrusive_ptr<repr>&>(o.m_rep)
    );

  return c;
}
