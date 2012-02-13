#include "analysis/pseries/repr.hpp"
#include "analysis/pseries/iterator.hpp"

#include <boost/functional/hash.hpp>

#include "util/foreach.hpp"

using namespace analysis;
using namespace pseries_detail;

std::size_t pseries::hash() const
{
  std::size_t seed = m_var.hash();

  foreach( const expr &ex, *this )
    boost::hash_combine( seed, ex.hash() );

  return seed;
}
