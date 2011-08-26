#ifndef UTILS_MOVE_HPP_
#define UTILS_MOVE_HPP_

#include<boost/config.hpp>

#if defined( BOOST_HAS_RVALUE_REFS ) // c++0x

#include <utility>

#else // c++03

#include<boost/type_traits/remove_reference.hpp>

namespace std {
  template<typename _Tp>
    inline _Tp&
    forward(typename boost::remove_reference<_Tp>::type& __t)
    { return __t; }

  template<typename _Tp>
    inline _Tp&
    move(_Tp& __t)
    { return __t; }

  template<typename _Tp>
    inline const _Tp&
    move(const _Tp& __t)
    { return __t; }
}

#endif

#include "util/null.hpp" // for nullptr
#include "util/refcounted.hpp" // for nonassignable
#include "util/assert.hpp"
#include "operators.hpp" // for testable

namespace util {

template<class T>
struct move_ptr
: private util::nonassignable
, public operators::testable< move_ptr<T> >
{
public: // cdtor
  move_ptr() throw()
  : m_ptr( nullptr ) {}

  move_ptr( T* p ) throw()
  : m_ptr( p ) {}

  move_ptr( move_ptr &o ) throw()
  : m_ptr( o.m_ptr )
  { o.m_ptr = nullptr; }

  void swap( move_ptr &o )
  { std::swap( m_ptr, o.m_ptr ); }

  ~move_ptr() throw()
  { delete m_ptr; }

public: // access
  T* get() const
  { return m_ptr; }

  T& operator*() const
  { ASSERT( m_ptr ); return *m_ptr; }

  T* operator->() const
  { ASSERT( m_ptr ); return  m_ptr; }

public: // modification
  void reset( T* p = nullptr )
  {
    ASSERT( p != m_ptr );
    delete m_ptr;
    m_ptr = p;
  }

  T* release()
  {
    T* ret = m_ptr;
    m_ptr = nullptr;
    return ret;
  }

public: // safe bool
  bool valid() const
  { return m_ptr != nullptr; }

private:
  T* m_ptr;
};

}

#endif /* UTILS_MOVE_HPP_ */
