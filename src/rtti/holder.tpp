#ifndef RTTI_HOLDER_TPP
#define RTTI_HOLDER_TPP

#include "rtti/holder.hpp"
#include "rtti/getter.hpp"

#include "util/assert.hpp"

#include <boost/type_traits/is_const.hpp>

#include "rtti/holder.hpp"

#include "rtti/creator.tpp"

namespace rtti {
namespace detail {

// ***** holder<>::initializer_t ***** //
template<class T, class Rt>
struct holder<T,Rt>::initializer_t
{
  initializer_t() { holder::assert_initialized(); }
  void touch() {}
};

// ***** holder<T,Rt> ***** //
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
  // won't be called for static ids
  node.id = creator<Rt>::template create<T>();
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
  creator<Rt>::template init_value<T>::value // static id or 0(undefined)
, &get_holder<super>::type::node // constant pointer
};
template<class T, class Rt>
typename holder<T,Rt>::initializer_t holder<T,Rt>::initializer;

template<class Rt>
const rtti_node holder<Rt,Rt>::node
  = { 0, 0 };

}} // namespace rtti::detail


#endif
