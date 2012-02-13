#ifndef ANALYSIS_PSERIES_HPP
#define ANALYSIS_PSERIES_HPP

#include "analysis/pseries/pseries.hpp"
#include "analysis/pseries/repr.hpp"

#include "analysis/pseries/iterator.hpp"

#ifndef PSERIES_IPP_FORCE
#define INLINE inline
#endif

namespace analysis {

INLINE pseries::const_iterator pseries::begin() const
{
  return const_iterator( (void*)m_rep->m_value );
}

} // namespace analysis

#ifdef INLINE
#undef INLINE
#endif

#endif
