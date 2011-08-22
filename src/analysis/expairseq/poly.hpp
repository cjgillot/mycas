#ifndef EXPAIRSEQ_POLY_HPP
#define EXPAIRSEQ_POLY_HPP

// TODO implement unsafe_vector as a deque with
// a pool for bucket allocation

#include <new>
#include <memory> // for allocator
#include <iterator> // for reverse_iterator

#include <boost/noncopyable.hpp>
#include <boost/pool/singleton_pool.hpp>

#include "util/null.hpp"

namespace analysis {
namespace epseq {
namespace detail {

// TODO implement portable iterators
template<class _Tp, class _Alloc = std::allocator<_Tp> >
class unsafe_vector
: private boost::noncopyable
, private _Alloc::template rebind<_Tp>::other {

  typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;

  MAKE_REFCOUNTED( unsafe_vector );

public: // member typedefs
  typedef _Tp                                         value_type;
  typedef typename _Tp_alloc_type::pointer            pointer;
  typedef typename _Tp_alloc_type::const_pointer      const_pointer;
  typedef typename _Tp_alloc_type::reference          reference;
  typedef typename _Tp_alloc_type::const_reference    const_reference;

  typedef std::size_t                                 size_type;
  typedef std::ptrdiff_t                              difference_type;

  typedef _Alloc                                      allocator_type;

  typedef pointer                               iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;

  typedef const_pointer                         const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

public: // cdtor
  explicit
  unsafe_vector(size_type nb)
  : _Tp_alloc_type()
  , m_number( nb )
  , m_start( nb != 0 ? _Tp_alloc_type::allocate( nb ) : 0 )
  , m_finish( m_start )
  {}

  unsafe_vector(size_type nb, const allocator_type &a)
  : _Tp_alloc_type( a )
  , m_number( nb )
  , m_start( nb != 0 ? _Tp_alloc_type::allocate( nb ) : 0 )
  , m_finish( m_start )
  {}

  unsafe_vector(size_type nb, const value_type &x)
  : _Tp_alloc_type()
  , m_number( nb )
  , m_start( nb != 0 ? _Tp_alloc_type::allocate( nb ) : 0 )
  , m_finish( m_start )
  {
    for(; nb != 0; --nb)
      push_back(x);
  }

  unsafe_vector(size_type nb, const value_type &x, const allocator_type &a)
  : _Tp_alloc_type( a )
  , m_number( nb )
  , m_start( nb != 0 ? _Tp_alloc_type::allocate( nb ) : 0 )
  , m_finish( m_start )
  {
    for(; nb != 0; --nb)
      push_back(x);
  }

  ~unsafe_vector() throw()
  {
    if( m_start )
    {
      truncate( m_start );
      _Tp_alloc_type::deallocate( m_start, m_number );
    }
  }

public: // access
  bool empty() const
  { return m_finish == m_start; }

  size_type size() const
  { return size_type( m_finish - m_start ); }

  allocator_type get_allocator() const
  { return *this; }

public: // iterators
  iterator begin()
  { return m_start; }

  const_iterator begin() const
  { return m_start; }

  iterator end()
  { return m_finish; }

  const_iterator end() const
  { return m_finish; }

  reverse_iterator rbegin()
  { return reverse_iterator( end() ); }

  const_reverse_iterator rbegin() const
  { return const_reverse_iterator( end() ); }

  reverse_iterator rend()
  { return reverse_iterator( begin() ); }

  const_reverse_iterator rend() const
  { return const_reverse_iterator( begin() ); }

public: // front/back
  reference front()
  { return *begin(); }

  const_reference front() const
  { return *begin(); }

  reference back()
  { return *( end() - 1 ); }

  const_reference back() const
  { return *( end() - 1 ); }

public: // modifiers
  void push_back(const value_type &x)
  {
    assert( size() < m_number );
    _Tp_alloc_type::construct( m_finish, x );
    ++m_finish;
  }

  void pop_back()
  {
    assert( ! empty() );
    --m_finish;
    _Tp_alloc_type::destroy( m_finish );
  }

  void truncate(const iterator &it)
  {
    assert( m_start <= it && std::size_t( it - m_start ) < m_number );
    if( m_finish < it )
      return;
    while( --m_finish != it )
      _Tp_alloc_type::destroy( m_finish );
  }

private: // data
  const size_type m_number;
  const pointer m_start;
  pointer m_finish;

public: // allocation operators
  static void* operator new     ( std::size_t ) throw( std::bad_alloc );
  static void  operator delete  ( void* )       throw();

  static void* operator new     ( std::size_t, std::nothrow_t ) throw();
  static void  operator delete  ( void*,       std::nothrow_t ) throw();

private: // disallow array operators
  static void* operator new   []( std::size_t ) throw();
  static void  operator delete[]( void* )       throw();
};

struct unsafe_vector_pool_tag {};

template<class _Tp, class _Alloc>
void* unsafe_vector<_Tp, _Alloc>::operator new( std::size_t n, std::nothrow_t ) throw()
{
  assert( n == sizeof( unsafe_vector ) );

  typedef boost::singleton_pool<unsafe_vector_pool_tag, sizeof( unsafe_vector )> pool_type;

  return pool_type::malloc();
}
template<class _Tp, class _Alloc>
void unsafe_vector<_Tp, _Alloc>::operator delete( void* p, std::nothrow_t ) throw()
{
  typedef boost::singleton_pool<unsafe_vector_pool_tag, sizeof( unsafe_vector )> pool_type;

  // is this necessary ?
  if( ! p ) return;

  assert( pool_type::is_from( p ) );
  pool_type::free( p );
}

template<class _Tp, class _Alloc>
void* unsafe_vector<_Tp, _Alloc>::operator new( std::size_t n ) throw( std::bad_alloc )
{
  void* p = unsafe_vector::operator new( n, std::nothrow );

  if( p ) return p;
  throw std::bad_alloc();
}
template<class _Tp, class _Alloc>
void unsafe_vector<_Tp, _Alloc>::operator delete( void* p ) throw()
{
  unsafe_vector::operator delete( p, std::nothrow );
}


}}} // namespace analysis::epseq::detail

#endif
