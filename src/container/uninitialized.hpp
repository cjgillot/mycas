#ifndef CONTAINER_UNINITIALIZED_HPP
#define CONTAINER_UNINITIALIZED_HPP

#include <memory>
#include "util/concept.hpp"

namespace container {

template< class Input, class Output, class Alloc >
Output uninitialized_copy(
  Input first
, const Input &last
, Output out
, Alloc &
) {
  CONCEPT_ASSERT(( boost::InputIterator< Input > ));
  CONCEPT_ASSERT(( boost::ForwardIterator< Output > ));
//   for(; first != last; ++first, ++out)
//     a.construct( &*out, *first );
//   return out;
  return std::uninitialized_copy(first, last, out);
}

template< class Output, class Value, class Alloc >
void uninitialized_fill_n(
  Output out
, std::size_t nb
, const Value &x
, Alloc &
) {
  CONCEPT_ASSERT(( boost::ForwardIterator< Output > ));
//   for(; nb != 0; --nb, ++out)
//     a.construct( &*out, x );
//   return out;
  std::uninitialized_fill_n(out, nb, x);
}

template< class Output, class Alloc >
void uninitialized_destroy(
  Output first
, const Output &last
, Alloc &
) {
  BOOST_CONCEPT_ASSERT(( boost::ForwardIterator< Output > ));
  typedef typename Alloc::value_type value_type;
  for(; first != last; ++first)
    first->~value_type();
//     a.destroy( &*first );
}

}

#endif
