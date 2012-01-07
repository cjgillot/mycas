#include "analysis/expand/fwd.hpp"

namespace analysis {
namespace expand_detail {

/*!\brief Expand a power of a sum
 *
 */
static ptr<const sum>
expand_sum_pow( const sum &a, unsigned long e )
{
  ptr<const sum> ret = sum::from_number( number::zero() );
  ptr<const sum> pow = a;

  for(;;)
  {
    if( e & 1 )
      ret = expand_sum_sum( *ret, *pow );

    e >>= 1;
    if( !e )
      break;

    expand_sum_sum( *pow, *pow );
  }

  return ret;
}

}} // namespace analysis::expand_detail
