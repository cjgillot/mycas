#ifndef EXPAIRSEQ_POLY_HPP
#define EXPAIRSEQ_POLY_HPP

// TODO implement poly as a deque with
// a pool for bucket allocation

#include "container/unsafe_vector.hpp"

#include <new>
#include <memory> // for allocator
#include <iterator> // for reverse_iterator

#include <boost/noncopyable.hpp>
#include <boost/pool/singleton_pool.hpp>

#include "util/null.hpp"
#include "util/assert.hpp"
#include "util/concept.hpp"

namespace analysis {
namespace epseq {

// TODO implement portable iterators
template<class _Tp, class _Alloc = std::allocator<_Tp> >
class poly
{
  typedef container::unsafe_vector<_Tp, _Alloc> vector_type;

  MAKE_REFCOUNTED( poly );

#define IMPORT( name ) \
  typedef typename vector_type::name name // ;

public: // member typedefs
  IMPORT( value_type      );
  IMPORT( pointer         );
  IMPORT( const_pointer   );
  IMPORT( reference       );
  IMPORT( const_reference );

  IMPORT( size_type       );
  IMPORT( difference_type );

  IMPORT( allocator_type );

  IMPORT( iterator                );
  IMPORT( reverse_iterator        );
  IMPORT( const_iterator          );
  IMPORT( const_reverse_iterator  );

#undef IMPORT

public: // cdtor
  explicit poly(size_type nb)
  : m_impl( nb ) {}

  poly(size_type nb, const allocator_type &a)
  : m_impl( nb, a ) {}

  poly(size_type nb, const value_type &x)
  : m_impl( nb, x ) {}

  poly(size_type nb, const value_type &x, const allocator_type &a)
  : m_impl( nb, x, a ) {}

  template<class Iterator>
  poly(std::size_t nb, Iterator first, const Iterator &last)
  : m_impl( nb, first, last ) {}

  ~poly() throw() {}

public: // access
  bool empty() const                    { return m_impl.empty(); }
  size_type size() const                { return m_impl.size(); }
  size_type max_size() const            { return m_impl.max_size(); }

public: // iterators
  iterator begin()                      { return m_impl.begin(); }
  const_iterator begin() const          { return m_impl.begin(); }
  iterator end()                        { return m_impl.end(); }
  const_iterator end() const            { return m_impl.end(); }
  reverse_iterator rbegin()             { return m_impl.rbegin(); }
  const_reverse_iterator rbegin() const { return m_impl.rbegin(); }
  reverse_iterator rend()               { return m_impl.rend(); }
  const_reverse_iterator rend() const   { return m_impl.rend(); }

public: // front/back
  reference front()                     { return m_impl.front(); }
  const_reference front() const         { return m_impl.front(); }
  reference back()                      { return m_impl.back(); }
  const_reference back() const          { return m_impl.back(); }

public: // modifiers
  void push_back(const value_type &x)
  { m_impl.push_back( x ); }

  template<class Iterator>
  void push_range(const Iterator &first, const Iterator &last)
  { m_impl.push_range( first, last ); }

  void pop_back()
  { m_impl.pop_back(); }

  void truncate(iterator it)
  { m_impl.truncate( it ); }

  void shrink()
  { m_impl.shrink(); }

private: // data
  vector_type m_impl;

public: // allocation operators
  static void* operator new     ( std::size_t ) throw( std::bad_alloc );
  static void  operator delete  ( void* )       throw();

  static void* operator new     ( std::size_t, std::nothrow_t ) throw();
  static void  operator delete  ( void*,       std::nothrow_t ) throw();

private: // disallow array operators
  static void* operator new   []( std::size_t ) throw();
  static void  operator delete[]( void* )       throw();
};

struct poly_pool_tag {};

template<class _Tp, class _Alloc>
void* poly<_Tp, _Alloc>::operator new( std::size_t n, std::nothrow_t ) throw()
{
  ASSERT( n == sizeof( poly ) );

  typedef boost::singleton_pool<poly_pool_tag, sizeof( poly )> pool_type;

  return pool_type::malloc();
}
template<class _Tp, class _Alloc>
void poly<_Tp, _Alloc>::operator delete( void* p, std::nothrow_t ) throw()
{
  typedef boost::singleton_pool<poly_pool_tag, sizeof( poly )> pool_type;

  // is this necessary ?
  if( ! p ) return;

  ASSERT( pool_type::is_from( p ) );
  pool_type::free( p );
}

template<class _Tp, class _Alloc>
void* poly<_Tp, _Alloc>::operator new( std::size_t n ) throw( std::bad_alloc )
{
  void* p = poly::operator new( n, std::nothrow );

  if( p ) return p;
  throw std::bad_alloc();
}
template<class _Tp, class _Alloc>
void poly<_Tp, _Alloc>::operator delete( void* p ) throw()
{
  poly::operator delete( p, std::nothrow );
}


}} // namespace analysis::epseq

#endif
