#include "analysis/pseries.hpp"
#include "analysis/pseries/repr.hpp"

using namespace analysis;
using namespace pseries_detail;

#define UNARY_OP( name, func )                    \
pseries pseries::name##_series(                   \
  const pseries &a                                \
) {                                               \
  pseries ret ( a.m_var );                        \
  ret.m_rep->m_value = pseries_export_##func(     \
    a.m_rep->m_value                              \
  );                                              \
  return ret;                                     \
}

#define BINARY_OP( name, func )                   \
pseries pseries::name##_series(                   \
  const pseries &a, const pseries &b              \
) {                                               \
  assert( a.m_var == b.m_var );                   \
  pseries ret ( a.m_var );                        \
  ret.m_rep->m_value = pseries_export_##func(     \
    a.m_rep->m_value                              \
  , b.m_rep->m_value                              \
  );                                              \
  return ret;                                     \
}

UNARY_OP( neg, neg )
UNARY_OP( inv, inv )

BINARY_OP( add, add )
BINARY_OP( sub, sub )

BINARY_OP( mul, mul )
BINARY_OP( div, div )
