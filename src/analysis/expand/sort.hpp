#ifndef ANALYSIS_EXPAND_SORT_HPP
#define ANALYSIS_EXPAND_SORT_HPP

#include "analysis/prod.hpp"
#include "analysis/sum.hpp"

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
inline void sort(const Iterator &beg, const Iterator &end)
{
  Iterator it = std::adjacent_find( beg, end, partial_not_pred() );
  if( it == end )
    util::insertion_sort( beg, end, sum::sort_predicate() );
  else
    util::timsort( beg, end, sum::sort_predicate() );
}

}} // namespace analysis::expand_detail

#endif
