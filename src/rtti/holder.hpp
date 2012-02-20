#ifndef RTTI_HOLDER_HPP
#define RTTI_HOLDER_HPP

#include <boost/type_traits/is_const.hpp>

#include "rtti/rttifwd.hpp"
#include "rtti/creator.hpp"
#include "rtti/getter.hpp"

#include "util/assert.hpp"

namespace rtti {
namespace detail {

/*
 * Current implementation :
 *
 * rtti_type is a native unsigned integer,
 * which range allows for at least 2 ^ 16 classes
 * in a hierarchy.
 *
 * The template detail::do_rtti_create holds
 * the current rtti id for each hierarchy.
 *
 * A new class id is just the next non-assigned
 * integer.
 */

//! \brief POD class holding a hierarchy bottom-up
struct rtti_node {
  rtti_type id;
  const rtti_node* const base;
};

//! Arguments must be const-qualified to avoid unnecessary instanciations
template<class T, class Rt>
struct holder
{
  static rtti_node node;

  static inline
  const rtti_node*
  ATTRIBUTE_PURE
  get_node()
  { return &node; }

  static inline
  rtti_type
  ATTRIBUTE_PURE
  get_id()
  { return node.id; }

  static void assert_initialized();

private:
  STATIC_ASSERT( boost::is_const<T>::value );
  STATIC_ASSERT( boost::is_const<Rt>::value );

  typedef typename RTTI_GETTER::traits<T>::super super;

  static void initialize();

  struct initializer_t;
  static initializer_t initializer;
};

template<class Rt>
struct holder<Rt,Rt> {

  static const rtti_node node;

  static inline
  const rtti_node*
  ATTRIBUTE_PURE
  get_node()
  { return &node; }

  static inline
  rtti_type
  ATTRIBUTE_CONST
  get_id()
  { return 0; }

  static void assert_initialized();

};

//! \brief Grant access to the holder
template<class T>
struct get_holder {
  typedef typename RTTI_GETTER::traits<T>::base Rt;
  typedef holder<const T, const Rt> type;
};

}} // namespace rtti::detail

#endif

// #include "rtti/holder.ipp"
