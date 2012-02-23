#ifndef UTIL_INSERTION_SORT_HPP
#define UTIL_INSERTION_SORT_HPP

#include <vector>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <utility>

#include "util/move.hpp"

namespace util {

namespace detail {

// /// This is a helper function for the sort routine.
// template<typename _RandomAccessIterator>
// void
// unguarded_linear_insert(_RandomAccessIterator last)
// {
//   typename iterator_traits<_RandomAccessIterator>::value_type
//     val = std::move(*last);
//
//   _RandomAccessIterator next = last;
//
//   --next;
//   while( val < *next )
//   {
//     *last = std::move( *next );
//     last = next;
//     --next;
//   }
//   *last = std::move( val );
// }

/// This is a helper function for the sort routine.
template<typename _RandomAccessIterator, typename _Compare>
void
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

// /// This is a helper function for the sort routine.
// template<typename _RandomAccessIterator>
// void
// insertion_sort(_RandomAccessIterator first,
//       _RandomAccessIterator last)
// {
//   if( first == last )
//     return;
//
//   for( _RandomAccessIterator i = first + 1; i != last; ++i )
//   {
//     if (*i < *first)
//       {
//         typename iterator_traits<_RandomAccessIterator>::value_type
//           val = _GLIBCXX_MOVE(*i);
//         _GLIBCXX_MOVE_BACKWARD3(first, i, i + 1);
//         *first = _GLIBCXX_MOVE(val);
//       }
//     else
//       std::unguarded_linear_insert(i);
//   }
// }

/// This is a helper function for the sort routine.
template<typename _RandomAccessIterator, typename _Compare>
void
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
      std::copy_backward( first, i, i + 1 );
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
