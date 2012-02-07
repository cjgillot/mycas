#include "analysis/vectorseq/expand/fwd.hpp"
#include "analysis/vectorseq/expand/heapmul.hpp"

#include "analysis/vectorseq/expand/sort.hpp"

namespace analysis {
namespace expand_detail {

/*!\brief Expand the product of two sums
 *
 * Complexity :
 * - multiplication : n^2
 * - comparison : n^2*lg(n)
 * - space : n
 */
ptr<const sum>
expand_sum_sum( const sum &a, const sum &b )
{
  // a = (+ ac a1 a2 ...)
  // b = (+ bc b1 b2 ...)

  typedef container::ptr_unsafe_vector< const prod > pvector_t;

  ASSERT( ! a.empty() && ! b.empty() );

  pvector_t seq ( ( a.size() + 1 ) * ( b.size() + 1 ) );

  const number &ac = a.coef();
  const number &bc = b.coef();

  { // hard multiplication work
    const prod::handle
      ach = prod::from_number( ac ),
      bch = prod::from_number( bc );
    expand_detail::expand_heap( seq, ach, a, bch, b );
  }

  sort( seq.begin(), seq.end() );

  ptr< const sum > retp ( sum::from_sorted_prod_range( ac * bc, seq.begin(), seq.end() ) );
  retp->basic::expand();
  return retp;
}

}} // namespace analysis::expand_detail
