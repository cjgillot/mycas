#ifndef RTTI_GETTER_IPP
#define RTTI_GETTER_IPP

#include "rtti/getter.hpp"
#include "rtti/holder.hpp"

namespace rtti {
namespace detail {

template<class T>
inline const rtti_node*
rtti_getter::static_node() throw()
{
  typedef typename boost::remove_pointer<T>::type T2;
  typedef typename RTTI_GETTER::types<T2>::base Root;
  typedef holder<T2, Root> h;
  h::assert_initialized();
  return h::get_node();
}

template<class T>
inline rtti_type
rtti_getter::static_id() throw()
{ return static_node<T>()->id; }


template<class T>
inline const rtti_node*
rtti_getter::get_node(const T &x) throw()
{ return x.rtti_get(); }

template<class T>
inline const rtti_node*
rtti_getter::get_node(const T* x)
  throw( std::bad_typeid )
{
  if( x )
    return x->rtti_get();
  //! follow builtin \c typeid behaviour :
  //! throw if null pointer
  throw std::bad_typeid();
}

template<class T>
inline rtti_type
rtti_getter::get_id(const T &x) throw()
{ return get_node( x )->id; }

template<class T>
inline rtti_type
rtti_getter::get_id(const T* x)
  throw( std::bad_typeid )
{ return get_node( x )->id; }

}} // namespace rtti::detail

#endif
