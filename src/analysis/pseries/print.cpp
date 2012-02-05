#include "analysis/pseries/repr.hpp"

#include "analysis/pseries/iterator.hpp"

using namespace analysis;
using namespace pseries_detail;

void pseries::print(std::ostream &os) const
{
  iterator it ( m_rep->m_value );

  if( !it.forced() || it.empty() )
  {
    os << "O(1)";
    return;
  }

  os << *it;
  ++it;

  if( !it.forced() || it.empty() )
  {
    os << " O(" << m_var << ')';
    return;
  }

  os << ' ' << *it << '@' << m_var;
  ++it;

  std::size_t n = 1;
  for( ; it.forced() && !it.empty(); ++it, ++n )
    os << ' ' << *it << '@' << m_var << '^' << n;

  os << " O(" << m_var << '^' << n << ')';
}
