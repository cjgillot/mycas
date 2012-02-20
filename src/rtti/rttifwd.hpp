#ifndef RTTI_FWD_HPP
#define RTTI_FWD_HPP

#define RTTI_GETTER \
  ::rtti::detail::rtti_getter

namespace rtti {
namespace detail {

//! \brief RTTI id type
typedef unsigned int rtti_type;

//! \brief RTTI node type
struct rtti_node;

//! \brief Friend structure for in-class rtti access
struct rtti_getter;

template<class T>
struct get_holder;

} // namespace detail

using detail::rtti_type;
using detail::rtti_node;

} // namespace rtti

#endif
