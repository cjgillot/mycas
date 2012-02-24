#ifndef ANALYSIS_EXPAND_SORT_HPP
#define ANALYSIS_EXPAND_SORT_HPP

#include "analysis/vectorseq.hpp"

#include "util/insertionsort.hpp"
#include "util/timsort.hpp"

namespace analysis {
namespace expand_detail {

struct partial_not_pred
: public std::binary_function<const prod*, const prod*, bool>
{
  bool operator()(const prod* a, const prod* b)
  {
    std::size_t ha, la;
    std::size_t hb, lb;
    a->vectorseq_base::sort_hash( ha, la );
    b->vectorseq_base::sort_hash( hb, lb );
    return (ha > hb) || ((ha == hb) & (la > lb));
  }
};

template<class Iterator>
inline void
sort(const Iterator &beg, const Iterator &end)
{
  std::size_t nruns = 0;

  Iterator it = beg;
  do
  { // create runs for timsort
    Iterator next = std::adjacent_find( it, end, partial_not_pred() );
    // insertion_sort should not behave too bad
    // on subranges already hash-sorted
    util::insertion_sort( it, next, sum::sort_predicate() );
    it = next; ++nruns;
  }
  while( it != end );

  if( nruns > 1 )
    util::timsort( beg, end, sum::sort_predicate() );
}

}} // namespace analysis::expand_detail

#endif
