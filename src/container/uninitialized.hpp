#ifndef CONTAINER_UNINITIALIZED_HPP
#define CONTAINER_UNINITIALIZED_HPP

#include "util/concept.hpp"

namespace container {

template< class Input, class Output, class Alloc >
Output uninitialized_copy(
  Input first
, const Input &last
, Output out
, Alloc &a
) {
  CONCEPT_ASSERT(( boost::InputIterator< Input > ));
  CONCEPT_ASSERT(( boost::ForwardIterator< Output > ));
  for(; first != last; ++first, ++out)
    a.construct( &*out, *first );
  return out;
}

template< class Output, class Value, class Alloc >
Output uninitialized_fill_n(
  Output out
, std::size_t nb
, const Value &x
, Alloc &a
) {
  CONCEPT_ASSERT(( boost::ForwardIterator< Output > ));
  for(; nb != 0; --nb, ++out)
    a.construct( &*out, x );
  return out;
}

template< class Output, class Alloc >
void uninitialized_destroy(
  Output first
, const Output &last
, Alloc &a
) {
  BOOST_CONCEPT_ASSERT(( boost::ForwardIterator< Output > ));
  for(; first != last; ++first)
    a.destroy( &*first );
}

}

#endif
