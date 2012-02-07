#include "analysis/vectorseq/expand/fwd.hpp"

namespace analysis {
namespace expand_detail {

ptr<const sum>
expand_sum_pow( const sum &a, unsigned long e )
{
  ptr<const sum> pow = &a;

  if( ! e )
    return sum::from_number( 1 );

  for(; ! (e & 1); e >>= 1 )
    pow = expand_sum_sum( *pow, *pow );

  // [e] odd
  ptr<const sum> ret = pow;
  e >>= 1;

  for(; e ; e >>= 1)
  {
    pow = expand_sum_sum( *pow, *pow );

    if( e & 1 )
      ret = expand_sum_sum( *ret, *pow );
  }

  return ret;
}

}} // namespace analysis::expand_detail
