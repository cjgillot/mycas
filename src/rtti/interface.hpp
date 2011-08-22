#ifndef RTTI_INTERFACE_HPP
#define RTTI_INTERFACE_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include "rtti/getter.ipp"

#include "util/attribute.hpp"
#include "util/compare.hpp"

#define RTTI_NO_DYNAMIC_CAST

/*
 * In this file, the [pure] attribute is
 * overused on purpose to create room for
 * as much optimization as possible.
 */

namespace rtti {

//! \brief RTTI id type
using detail::rtti_type;

//! \brief Function for comparision of RTTI ids
inline util::cmp_t ATTRIBUTE_PURE
rtti_compare( rtti_type a, rtti_type b )
{ return util::compare(a,b); }

//! \brief Function checking exact type
template<class T, class U>
inline bool ATTRIBUTE_PURE
is_exactly_a(const U &x)
{
  BOOST_STATIC_ASSERT(( boost::is_same<
    typename RTTI_GETTER::types<T>::base
  , typename RTTI_GETTER::types<U>::base
  >::value &&
    "is_exactly_a should be called"
    "inside an unique hierarchy"
  ));

  return RTTI_TYPE_ID( T ) == RTTI_ID( x );
}

//! \brief Function checking exact type, pointer version
template<class T, class U>
inline bool ATTRIBUTE_PURE ATTRIBUTE_NONNULL(1)
is_exactly_a(const U* x)
{
  // reject pointer-to-pointer
  BOOST_STATIC_ASSERT( ! boost::is_pointer<U>::value );
  typedef typename boost::remove_pointer<T>::type T2;

  // retry without pointer
  return rtti::is_exactly_a<T2, U>( *x );
}

#ifndef RTTI_NO_DYNAMIC_CAST

template<class T, class U>
inline bool ATTRIBUTE_PURE
is_a(const U &x)
{ return dynamic_cast<const T*>( &x ) != 0; }

template<class T, class U>
inline bool ATTRIBUTE_PURE
is_a(const U* x)
{
  typedef typename boost::remove_pointer<T>::type T2;
  return dynamic_cast<const T2*>( x ) != 0;
}

#else

namespace detail {

// T and U shall be const-qualified raw class types (no reference, no pointers)
template<class T, class U>
struct is_a_impl {
  static inline bool ATTRIBUTE_PURE
  apply(const U &x)
  {
    const ::rtti::rtti_type id = RTTI_TYPE_ID( T );
    const ::rtti::rtti_node* n = RTTI_NODE( x );

    do {
      // got it ?
      if( n->id == id )
        return true;

      // upcast
      n = n->base;
    } while( n );
    return false;
  }
};

template<class T>
struct is_a_impl<T, T> {
  static inline bool ATTRIBUTE_CONST
  apply(const T &)
  { return true; }
};

}

template<class T, class U>
inline bool ATTRIBUTE_PURE
is_a(const U &x)
{
  // U shall be a raw class
  typedef typename boost::remove_reference<T>::type T2;
  typedef typename boost::remove_pointer <T2>::type T3;
  return detail::is_a_impl<const T3, const U>::apply( x );
}

template<class T, class U>
inline bool ATTRIBUTE_PURE
is_a(const U* x)
{ return x == 0 ? false : is_a<T>( *x ); }

#endif

} // namespace rtti

#endif
