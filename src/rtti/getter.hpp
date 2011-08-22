#ifndef RTTI_GETTER_HPP
#define RTTI_GETTER_HPP

#include <typeinfo>

#include "util/attribute.hpp"

#include "rtti/rttifwd.hpp"

#define RTTI_GETTER \
  ::rtti::detail::rtti_getter

//! \brief access an object RTTI node
#define RTTI_NODE( x ) \
  RTTI_GETTER::get_node( x )

//! \brief access an object RTTI id
#define RTTI_ID( x ) \
  RTTI_GETTER::get_id( x )

//! \brief access a type RTTI node
#define RTTI_TYPE_NODE( T ) \
  RTTI_GETTER::static_node<T>()

//! \brief access a type RTTI id
#define RTTI_TYPE_ID( T ) \
  RTTI_GETTER::static_id<T>()

namespace rtti {
namespace detail {

//! \brief Friend structure for in-class rtti access
struct rtti_getter {

  //! \brief Access member types
  template<class T>
  struct types {
    typedef typename
      T::rtti_base_type
        base;
    typedef typename
      T::rtti_super_type
        super;
    typedef typename
      T::rtti_self_type
        self;
  };

  //! \brief Get static id
  template<class T>
  static inline const rtti_node*
  ATTRIBUTE_PURE
  static_node() throw();

  //! \brief Get static id
  template<class T>
  static inline rtti_type
  ATTRIBUTE_PURE
  static_id() throw();

  //! \brief Get object node
  template<class T>
  static inline const rtti_node*
  ATTRIBUTE_PURE
  get_node(const T &x) throw();

  //! \brief Get pointer node
  template<class T>
  static inline const rtti_node*
  ATTRIBUTE_PURE
  ATTRIBUTE_NONNULL(1)
  get_node(const T* x) throw( std::bad_typeid );

  //! \brief Get object id
  template<class T>
  static inline rtti_type
  ATTRIBUTE_PURE
  get_id(const T &x) throw();

  //! \brief Get pointer id
  template<class T>
  static inline rtti_type
  ATTRIBUTE_PURE
  ATTRIBUTE_NONNULL(1)
  get_id(const T* x) throw( std::bad_typeid );

};

}} // namespace rtti::detail

#endif
