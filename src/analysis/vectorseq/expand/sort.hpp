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

template<typename Iterator>
inline void
sort(const Iterator &beg, const Iterator &end)
{
  /*
   * The range [beg, end) has a very special structure, coming from
   * the heap multiplication algorithm.
   *
   * It is mainly a sequence of one or more "almost sorted" sequences,
   * which come from overflow in hash.
   * Each "almost sorted" sequence is sorted wrt the hash,
   * but not wrt the complete predicate, so insertion sort
   * is efficient (small movements).
   * Then, some merge does the trick.
   *
   * For now, we use timsort algorithm, because std::inplace_merge
   * rejects the iterator type.
   */

  // select first "almost sorted" range
  Iterator mid = std::adjacent_find( beg, end, partial_not_pred() );
  util::insertion_sort( beg, mid, sum::sort_predicate() );

  do
  { // select next "almost sorted" range
    Iterator next = std::adjacent_find( mid, end, partial_not_pred() );
    util::insertion_sort( mid, next, sum::sort_predicate() );
    mid = next;
  }
  while(mid != end);

  util::timsort( beg, end, sum::sort_predicate() );
}

}} // namespace analysis::expand_detail

#endif
