#include "analysis/expand/fwd.hpp"
#include "analysis/expand/heapmul.hpp"

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

  // to be initialized later, at the end of next block
  ptr< sum > retp;

  { // sum by coefficient scaling
    const number &ac = a.coef();
    const number &bc = b.coef();

    // scaled products
    pvector_t scaled ( a.size() + b.size() );

    if( ! ac.null() )
    {
      if( ac.unit() )
        scaled.push_range( b.begin(), b.end() );
      else {
        util::scoped_ptr< sum > sp ( sum::sca( ac, b ) );
        scaled.push_range( sp->begin(), sp->end() );
      }
    }
    if( ! bc.null() )
    {
      if( bc.unit() )
        scaled.push_range( a.begin(), a.end() );
      else {
        util::scoped_ptr< sum > sp ( sum::sca( bc, a ) );
        scaled.push_range( sp->begin(), sp->end() );
      }
    }

    // create sum, n * lg n time
    retp.reset(
      sum::from_mutable_prod_range(
        ac * bc
      , scaled.begin()
      , scaled.end()
      )
    );
  }

  { // hard multiplication work
    pvector_t seq ( a.size() * b.size() );
    expand_detail::expand_heap( seq, a, b );

    // commit the work
    util::scoped_ptr< sum > tmp ( sum::from_mutable_prod_range( number::one(), seq.begin(), seq.end() ) );
    retp.reset( sum::add( *retp, *tmp ) );
  }

  retp->basic::expand();
  return retp;
}

}} // namespace analysis::expand_detail
