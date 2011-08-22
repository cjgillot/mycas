#ifndef RTTI_HOLDER_HPP
#define RTTI_HOLDER_HPP

#include <cassert>

#include "rtti/getter.hpp"

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

//! \brief RTTI id type
typedef unsigned int rtti_type;

//! \brief POD class holding a hierarchy bottom-up
struct rtti_node {
  rtti_type id;
  const rtti_node* const base;
};

template<class Rt>
class creator {

  static rtti_type current;

public:
  static inline
  rtti_type
  create() {
    // check for overflow
    assert(
      current != 0
    && "Too many instanciated classes for current RTTI."
    );
    return current++;
  }
};

template<class T, class Rt>
struct holder {

  typedef typename RTTI_GETTER::types<T>::super super;

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

  static inline
  void assert_initialized()
  {
    initializer.touch();
    if( get_id() == 0 )
      initialize();
  }

private:
  static inline
  void initialize()
  {
    // scope lock a mutex if needed
    node.id = creator<Rt>::create();
  }

  struct initializer_t {
    initializer_t() { holder::assert_initialized(); }
    void touch() {}
  } static initializer;
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

  static inline
  void
  ATTRIBUTE_CONST
  assert_initialized()
  {}
};

// initialize with 1, base class has static 0 id
template<class Rt>
rtti_type creator<Rt>::current = 1;

// an POD aggregate initialized with a constant expression
// standard ensures initialization before any non-trivial constructor called
template<class T, class Rt>
rtti_node holder<T,Rt>::node = {
  0
, &holder<super, Rt>::node
};

template<class Rt>
const rtti_node holder<Rt,Rt>::node
  = { 0, 0 };

template<class T, class Rt>
typename holder<T,Rt>::initializer_t holder<T,Rt>::initializer;

template<class T>
struct get_holder {
  typedef typename RTTI_GETTER::types<T>::base Rt;
  typedef holder<T, Rt> type;
};

}} // namespace rtti::detail

#endif
