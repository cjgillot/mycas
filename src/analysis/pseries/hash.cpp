#include "analysis/pseries/repr.hpp"

#include "analysis/pseries/iterator.hpp"

using namespace analysis;
using namespace pseries_detail;

std::size_t pseries::hash() const
{
  std::size_t seed = m_var.hash();

  iterator it ( m_rep->m_value );

  for( ; it.forced() && !it.empty(); ++it )
    boost::hash_combine( seed, it->hash() );

  return seed;
}
