#ifndef CONTAINER_UVECTOR_HPP
#define CONTAINER_UVECTOR_HPP

#include <memory> // for allocator
#include <iterator> // for reverse_iterator

#include <boost/noncopyable.hpp>
#include <boost/pool/singleton_pool.hpp>

#include "util/assert.hpp"
#include "util/concept.hpp"

#include "container/allocator.hpp"
#include "container/uninitialized.hpp"

namespace container {

template< class _Tp, class _Alloc = std::allocator<_Tp> >
class unsafe_vector
// : private util::implement_concept< boost::Container<unsafe_vector<_Tp, _Alloc> > >
: private _Alloc::template rebind<_Tp>::other {

  typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;

public: // member typedefs
  typedef _Tp value_type;

#define IMPORT( name )  \
  typedef typename _Tp_alloc_type::name name // ;

  IMPORT( pointer         );
  IMPORT( const_pointer   );
  IMPORT( reference       );
  IMPORT( const_reference );

#undef IMPORT

  typedef std::size_t     size_type;
  typedef std::ptrdiff_t  difference_type;

  typedef _Tp_alloc_type  allocator_type;

  typedef pointer                               iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;

  typedef const_pointer                         const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

protected:
        allocator_type &get_allocator()       { return *this; }
  const allocator_type &get_allocator() const { return *this; }

public: // copy/move cdtor
  unsafe_vector( const unsafe_vector &o )
  : _Tp_alloc_type( o.get_allocator() )
  , m_number( o.m_number )
  , m_start( m_number ? _Tp_alloc_type::allocate( m_number ) : 0 )
  , m_finish( m_start + o.size() )
  {
    if( m_number )
      container::uninitialized_copy( o.begin(), o.end(), m_start, get_allocator() );
  }

  unsafe_vector( unsafe_vector&& o )
  : _Tp_alloc_type( std::move( o.get_allocator() ) )
  , m_number( std::move( o.m_number ) )
  , m_start( std::move( o.m_start ) )
  , m_finish( std::move( o.m_finish ) )
  {
    o.m_start = o.m_finish = nullptr;
    o.m_number = 0;
  }

  unsafe_vector& operator=( const unsafe_vector &o )
  {
    if( &o == this )
      return *this;

    if( o.m_number )
      // copy and swap
      unsafe_vector( o ).swap( *this );
    else
    {
      m_start = m_finish = nullptr;
      m_number = 0;
    }

    return *this;
  }

  unsafe_vector& operator=( unsafe_vector &&o )
  {
    if( &o == this )
      return *this;

    this->_Tp_alloc_type::operator=( std::move(o) );

    m_start = o.m_start;
    m_finish = o.m_finish;
    m_number = o.m_number;

    o.m_start = o.m_finish = nullptr;
    o.m_number = 0;

    return *this;
  }

  void swap( unsafe_vector &o ) throw()
  {
    detail::alloc_swap<_Tp_alloc_type>::apply( *this, o );
    std::swap( m_number, o.m_number );
    std::swap( m_start, o.m_start );
    std::swap( m_finish, o.m_finish );
  }

  ~unsafe_vector() throw()
  { clear(); }

public: // special constructors
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
  , m_finish( m_start + nb )
  {
    container::uninitialized_fill_n( m_start, nb, x, get_allocator() );
  }

  unsafe_vector(size_type nb, const value_type &x, const allocator_type &a)
  : _Tp_alloc_type( a )
  , m_number( nb )
  , m_start( nb != 0 ? _Tp_alloc_type::allocate( nb ) : 0 )
  , m_finish( m_start + nb )
  {
    container::uninitialized_fill_n( m_start, nb, x, get_allocator() );
  }

  template<class Iterator>
  unsafe_vector(std::size_t nb, Iterator first, const Iterator &last)
  : _Tp_alloc_type()
  , m_number( nb )
  , m_start( nb != 0 ? _Tp_alloc_type::allocate( m_number ) : 0 )
  , m_finish( m_start + nb )
  {
    CONCEPT_ASSERT(( boost::InputIterator<Iterator> ));
    ASSERT( nb == std::distance( first, last ) );
    container::uninitialized_copy( first, last, m_start, get_allocator() );
  }

  template<class Iterator>
  unsafe_vector(std::size_t nb, Iterator first, const Iterator &last, const allocator_type &a)
  : _Tp_alloc_type( a )
  , m_number( nb )
  , m_start( nb != 0 ? _Tp_alloc_type::allocate( m_number ) : 0 )
  , m_finish( m_start + nb )
  {
    CONCEPT_ASSERT(( boost::InputIterator<Iterator> ));
    ASSERT( nb == std::distance( first, last ) );
    container::uninitialized_copy( first, last, m_start, get_allocator() );
  }

public: // access
  bool      empty()     const { return m_finish == m_start; }
  size_type size()      const { return size_type( m_finish - m_start ); }
  size_type max_size()  const { return m_number; }

public: // iterators
  iterator begin()                      { return m_start; }
  iterator end()                        { return m_finish; }

  const_iterator begin() const          { return m_start; }
  const_iterator end() const            { return m_finish; }

  reverse_iterator rbegin()             { return reverse_iterator( end() ); }
  reverse_iterator rend()               { return reverse_iterator( begin() ); }

  const_reverse_iterator rbegin() const { return const_reverse_iterator( end() ); }
  const_reverse_iterator rend() const   { return const_reverse_iterator( begin() ); }

public: // front/back
  reference front()               { return *begin(); }
  reference back()                { return *( end() - 1 ); }

  const_reference front() const   { return *begin(); }
  const_reference back() const    { return *( end() - 1 ); }

public: // random access
  reference       operator[](std::size_t n)       { return m_start[n]; }
  const_reference operator[](std::size_t n) const { return m_start[n]; }

  reference       at(std::size_t n)       { rangecheck(n); return operator[](n); }
  const_reference at(std::size_t n) const { rangecheck(n); return operator[](n); }

private: // check range (may be private because it is static)
  static void rangecheck(size_type i)
  {
    if( i >= size() )
      throw std::out_of_range("unsafe_vector<>: index out of range");
  }

public: // modifiers
  void push_back(const value_type &x)
  {
    ASSERT( size() < m_number );
    get_allocator().construct( m_finish, x );
    ++m_finish;
  }

  template<typename... Args>
  void emplace_back(Args&& ...args)
  {
    ASSERT( size() < m_number );
    get_allocator().construct( m_finish, std::forward<Args>(args)... );
    ++m_finish;
  }

  template<class Iterator>
  void push_range(const Iterator &first, const Iterator &last)
  {
    CONCEPT_ASSERT(( boost::InputIterator<Iterator> ));
    ASSERT( std::size_t( std::distance( first, last ) ) <= m_number - size() );
    m_finish = container::uninitialized_copy( first, last, m_finish, get_allocator() );
  }

  void pop_back()
  {
    ASSERT( ! empty() );
    --m_finish;
    _Tp_alloc_type::destroy( m_finish );
  }

  void truncate(const iterator &it)
  {
    ASSERT( m_start <= it && std::size_t(it - m_start) < m_number );
    if( m_finish < it )
      return;
    container::uninitialized_destroy( it, m_finish, get_allocator() );
  }

  void shrink()
  {} // no-op since no constant-time implementation is available

  void clear()
  {
    if( m_start )
    {
      truncate( m_start );
      get_allocator().deallocate( m_start, m_number );
      m_number = 0;
      m_start = m_finish = nullptr;
    }
  }

private: // data
  size_type m_number;
  pointer m_start, m_finish;
};

} // namespace container

namespace std {

template<class _Tp, class _Alloc>
void swap(
  container::unsafe_vector<_Tp, _Alloc> &a
, container::unsafe_vector<_Tp, _Alloc> &b
)
{ a.swap( b ); }

}

#endif
