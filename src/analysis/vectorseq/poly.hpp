#ifndef EXPAIRSEQ_POLY_HPP
#define EXPAIRSEQ_POLY_HPP

// TODO implement poly as a deque with
// a pool for bucket allocation

#include "container/unsafe_vector.hpp"
#include "container/ptr_container.hpp"

#include <new>
#include <memory> // for allocator
#include <iterator> // for reverse_iterator

#include "util/assert.hpp"
#include "util/refcounted.hpp"

namespace analysis {
namespace vseq {

template<class _Tp, class _Alloc>
class poly
{
  typedef container::ptr_unsafe_vector<_Tp, _Alloc> vector_type;

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
  : m_impl( nb )              { ASSERT( nb ); }

  poly(size_type nb, const allocator_type &a)
  : m_impl( nb, a )           { ASSERT( nb ); }

  poly(size_type nb, const value_type &x)
  : m_impl( nb, x )           { ASSERT( nb ); }

  poly(size_type nb, const value_type &x, const allocator_type &a)
  : m_impl( nb, x, a )        { ASSERT( nb ); }

  template<class Iterator>
  poly(size_type nb, const Iterator &first, const Iterator &last)
  : m_impl( nb, first, last ) { ASSERT( nb ); }

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

}} // namespace analysis::vseq


#include "analysis/vectorseq/poly.tpp"
#endif
