#include "rtti/holder.hpp"

#include "rtti/visitor.hpp"

#include <algorithm>

using rtti::rtti_type;
using rtti::detail::rtti_node;
using rtti::visitor::detail::vtable_node;

typedef vtable_node::invoker_t invoker_t;

namespace {

//! \brief lower_bound specialization for vtables
//! \param first range begin
//! \param last range end
//! \param id : the id to be searched
//!
//! The two pointers must form a valid <em>[first, last)</em> range.
//!
//! \return \c ret verifying <em>[first,ret) < id</em> and <em>*ret > id</em>
const vtable_node*
search(
  const vtable_node* first
, const vtable_node* last
, const rtti_type id
) {
  assert( first < last );
  std::size_t len = std::size_t( last - first );

  while( len != 0 )
  {
    const std::size_t half = len >> 1;
    const vtable_node* const middle = first + half;

    if( middle->id < id )
    {
      first = middle + 1;
      len  -= half + 1;
    }
    else
      len   = half;
  }
  return first;
}

}

invoker_t
rtti::visitor::detail::fetch_node(
  const vtable_node* const beg
, const std::size_t vsz
, const rtti_node* rt
) throw() {
  //! issue a binary search for id in the \c vtable,
  //! with an upcast for each failure

  //! worst case time : min( vsz, h ) * lg( vsz )
  //!   with h the inheritance heigth

  //! best case time : 1

  //! usual case time : lg( vsz )

  // macros stopping iteration
#define SUCCESS() \
  return last->invoker

#define FAILURE() \
  return beg->invoker

  register rtti_type id = rt->id;

  // trivial case
  if( id == 0 ) FAILURE();

  // minimal search area
  // because ids are unique
  const vtable_node* last =
    ( id < vsz )
  ? ( beg + id )
  : ( beg + vsz - 1 );

  //! \invariant:
  //!   \c beg < \c last         [beg, last] is a valid range
  //!   \c rt && \c rt->id != 0  fallback  case not reached
  //! worst case loop termination :
  //!   there is a finite number of upcasts from \c rt up to the base class
  //!   and [beg, last] is a finite range
  //! usual loop termination :
  //!   something found in early steps
  for(;;)
  {
    const rtti_type vid = last->id;

    // singleton range
    if( vid == 0 ) FAILURE();

    // upcast if possible
    while( vid < id ) {
      rt = rt->base;
      id = rt->id;

      // base class reached
      if( id == 0 ) FAILURE();
    }

    // match found
    if( vid == id ) SUCCESS();

    // we have id < vid, shrink range
    last = search( beg, last, id );

    // got it !
    if( last->id == id )  SUCCESS();

    // no match
    if( last - beg <= 1 ) FAILURE();

    --last;
    // search invariant : last->id < id

    // upcast once
    rt = rt->base;
    id = rt->id;
    if( id == 0 ) FAILURE();
  }

#undef HIT
#undef FAILURE

}
