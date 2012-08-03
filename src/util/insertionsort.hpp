#ifndef UTIL_INSERTION_SORT_HPP
#define UTIL_INSERTION_SORT_HPP

#include <vector>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <utility>

namespace util {

namespace detail {

/// This is a helper function for the sort routine.
template<typename _RandomAccessIterator, typename _Compare>
static inline void
unguarded_linear_insert( _RandomAccessIterator last, _Compare comp )
{
  typename std::iterator_traits<_RandomAccessIterator>::value_type
    val = std::move(*last);

  _RandomAccessIterator next = last;

  --next;
  while( comp(val, *next) )
  {
    *last = std::move( *next );
    last = next;
    --next;
  }
  *last = std::move( val );
}

template<typename _RandomAccessIterator, typename _Compare>
inline void
insertion_sort(_RandomAccessIterator first,
      const _RandomAccessIterator &last, _Compare comp)
{
  if( first == last ) return;

  for( _RandomAccessIterator i = first + 1; i != last; ++i )
  {
    if( comp(*i, *first) )
    {
      typename std::iterator_traits<_RandomAccessIterator>::value_type
        val = std::move(*i);
      std::move_backward( first, i, i + 1 );
      *first = std::move(val);
    }
    else
      unguarded_linear_insert(i, comp);
  }
}

} // namespace detail

using detail::insertion_sort;

} // namespace util

#endif
