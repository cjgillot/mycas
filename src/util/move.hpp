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
struct scoped_ptr
: private util::noncopyable
, public operators::testable< scoped_ptr<T> >
{
public: // cdtor
  scoped_ptr() throw()
  : m_ptr( nullptr ) {}

  scoped_ptr( T* p ) throw()
  : m_ptr( p ) {}

  void swap( scoped_ptr &o ) throw()
  { std::swap( m_ptr, o.m_ptr ); }

  ~scoped_ptr() throw()
  { delete m_ptr; }

  scoped_ptr& operator=( T* p )
  { reset( p ); }

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
    T* const ret = m_ptr;
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
