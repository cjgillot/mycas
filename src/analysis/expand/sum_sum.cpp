#include "analysis/expand/fwd.hpp"
#include "analysis/expand/heapmul.hpp"

#include "util/insertionsort.hpp"

namespace analysis {
namespace expand_detail {

#if 0

template<class RRange, class Iter>
void scale_range(
    RRange &ret,
    const number &n,
    Iter beg, const Iter &end
) {
  for(; beg != end; ++beg )
  {
    const prod::handle h = *beg;
    ret.push_back( h.sca( n ).get() );
  }
}

#endif

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

#if 0
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
#endif

  { // hard multiplication work
    const prod::handle
      ach = prod::from_number( ac ),
      bch = prod::from_number( bc );
    expand_detail::expand_heap( seq, ach, a, bch, b );
  }

  // Insertion sort seems best here since
  // the heap expansion almost sorts the sequence
  // The non-sorted part calls for deep monomial comparison,
  // which seems long for any algorithm
  util::insertion_sort( seq.begin(), seq.end(), sum::sort_predicate() );

  ptr< const sum > retp ( sum::from_sorted_prod_range( ac * bc, seq.begin(), seq.end() ) );
  retp->basic::expand();
  return retp;
}

}} // namespace analysis::expand_detail
