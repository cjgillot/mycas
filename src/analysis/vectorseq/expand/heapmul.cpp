#include "analysis/vectorseq/expand/heapmul.hpp"

namespace analysis {
namespace expand_detail {

typedef container::ptr_unsafe_vector<const prod> RRange;

/*!\brief Heap multiplication
 *
 * Complexity ( n < m )
 * - Multiplication : n*m
 * - Comparison : n*m*lg(n)
 * - Space : n
 */
void expand_heap(
    RRange &ret,
    const number &f0, const sum &F,
    const number &g0, const sum &G
) {
  std::size_t lenf = F.size();
  std::size_t leng = G.size();
  if( lenf > leng )
  {
    expand_heap( ret, g0, G, f0, F );
    return;
  }

  heap_nmerge<sum::const_iterator> heap { lenf + 2 };

  {
    if( !f0.null() )
      heap.add_one( prod::from_number(f0), G.begin(), G.end() );
    if( !g0.null() )
      heap.add_one( prod::from_number(g0), F.begin(), F.end() );
  }
  for(const prod* f : F)
    heap.add_one( f, G.begin(), G.end() );

  heap.main(ret);
}

}} // namespace analysis::expand_detail
