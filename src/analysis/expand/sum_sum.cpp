#include "analysis/expand/fwd.hpp"
#include "analysis/expand/heapmul.hpp"

namespace analysis {
namespace expand_detail {

template<class RRange, class Iter>
void scale_range( RRange &ret, const number &n, Iter beg, const Iter &end )
{
  for(; beg != end; ++beg )
  {
    const prod::handle h = *beg;
    ret.push_back( h.sca( n ).get() );
  }
}

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

  { // sum by coefficient scaling
    const number &ac = a.coef();
    const number &bc = b.coef();

    if( ! ac.null() )
    {
      if( ac.unit() )
        seq.push_range( b.begin(), b.end() );
      else
        scale_range( seq, ac, b.begin(), b.end() );
    }
    if( ! bc.null() )
    {
      if( bc.unit() )
        seq.push_range( a.begin(), a.end() );
      else
        scale_range( seq, bc, a.begin(), a.end() );
    }
  }

  // hard multiplication work
  expand_detail::expand_heap< prod::handle >( seq, a, b );

  ptr< const sum > retp ( sum::from_mutable_prod_range( 0, seq.begin(), seq.end() ) );
  retp->basic::expand();
  return retp;
}

}} // namespace analysis::expand_detail
