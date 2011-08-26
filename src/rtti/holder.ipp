#ifndef RTTI_HOLDER_IPP
#define RTTI_HOLDER_IPP

#include "rtti/holder.hpp"
#include "rtti/getter.hpp"

#include "util/assert.hpp"

#include <boost/type_traits/is_const.hpp>

namespace rtti {
namespace detail {

// ***** creator ***** //

template<class Rt>
inline rtti_type
creator<Rt>::create() {
  // check for overflow
  ASSERT_MSG(
    current != 0
  , "Too many instanciated classes for current RTTI."
  );
  return current++;
}

// initialize with 1, base class has static 0 id
template<class Rt>
rtti_type creator<Rt>::current = 1;

// ***** holder ***** //

template<class T, class Rt>
inline void
holder<T, Rt>::assert_initialized()
{
  // avoid the initializer to be optimized out
  initializer.touch();
  if( get_id() == 0 )
    initialize();
}

template<class T, class Rt>
inline void
holder<T, Rt>::initialize()
{
  // scope lock a mutex if needed
  node.id = creator<Rt>::create();
}

// ***** holder<Rt,Rt> ***** //

template<class Rt>
inline void
holder<Rt, Rt>::assert_initialized()
{}

// an POD aggregate initialized with a constant expression
// standard ensures initialization before any non-trivial constructor called
template<class T, class Rt>
rtti_node holder<T,Rt>::node = {
  0 // undefined id
, &get_holder<super, Rt>::type::node // constant pointer
};
template<class T, class Rt>
typename holder<T,Rt>::initializer_t holder<T,Rt>::initializer;

template<class Rt>
const rtti_node holder<Rt,Rt>::node
  = { 0, 0 };

}} // namespace rtti::detail


#endif
