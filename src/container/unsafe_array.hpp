#ifndef CONTAINER_UNSAFE_ARRAY_HPP
#define CONTAINER_UNSAFE_ARRAY_HPP

#include <boost/type_traits/alignment_of.hpp>
#include <boost/aligned_storage.hpp>

#include "util/assert.hpp"
#include "util/concept.hpp"
#include "util/refcounted.hpp"

#include "container/allocator.hpp"

namespace container {

namespace detail {

template<class T>
struct make_storage {

  typedef ::boost::aligned_storage<
    sizeof( T )
  , boost::alignment_of<T>::value
  > type;

};

} // namespace detail

template<
  class _Tp
, std::size_t N
, class _Alloc = std::allocator<_Tp>
>
class unsafe_array
: private util::implement_concept< boost::Container<unsafe_array<_Tp, N, _Alloc> > >
, private _Alloc::template rebind<_Tp>::other
, private detail::make_storage<_Tp[N]>::type {

  typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;
  typedef typename detail::make_storage<_Tp[N]>::type storage_type;

  STATIC_ASSERT( N > 0 );

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

  typedef _Alloc          allocator_type;

  typedef pointer                               iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;

  typedef const_pointer                         const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
  pointer       address()       { return static_cast<      pointer>( storage_type::address() ); }
  const_pointer address() const { return static_cast<const_pointer>( storage_type::address() ); }

public:
        allocator_type &get_allocator()       { return *this; }
  const allocator_type &get_allocator() const { return *this; }

public: // cdtor
  unsafe_array(const unsafe_array &o)
  : _Tp_alloc_type( o.get_allocator() ), storage_type()
  {
    pointer        cur =   address();
    const_pointer ocur = o.address();

    for(
      std::size_t cnt = N;
      cnt != 0;
      --cnt, ++cur, ++ocur
    )
      get_allocator().construct( cur, *ocur );
  }

  unsafe_array& operator=(const unsafe_array &o)
  {
    get_allocator() = o.get_allocator();

    pointer        cur =   address();
    const_pointer ocur = o.address();

    for(
      std::size_t cnt = N;
      cnt != 0;
      --cnt, ++cur, ++ocur
    )
      *cur = *ocur;

    return *this;
  }

  void swap(unsafe_array &o)
  {
    detail::alloc_swap<_Tp_alloc_type>::apply( get_allocator(), o.get_allocator() );

    pointer
       cur =   address()
    , ocur = o.address();

    for(
      std::size_t cnt = N;
      cnt != 0;
      --cnt, ++cur, ++ocur
    )
      std::swap( *cur, *ocur );
  }

  ~unsafe_array() throw()
  {
    pointer cur = address();

    for(
      std::size_t cnt = N;
      cnt != 0;
      --cnt, ++cur
    )
      get_allocator().destroy( cur );
  }

public: // constructors from range
  template< class Iterator >
  unsafe_array(Iterator b, const Iterator &e)
  : _Tp_alloc_type(), storage_type()
  {
    ASSERT( std::distance( b, e ) == N ); (void)e;

    pointer cur = address();

    for(
      std::size_t cnt = N;
      cnt != 0;
      --cnt, ++b, ++cur
    ) {
      // redundant with the distance assert
      ASSERT( b != e );

      get_allocator().construct( cur, *b );
    }

    // redundant with the distance assert
    ASSERT( b == e );
  }

  template< class Iterator >
  unsafe_array(Iterator b, const Iterator &e, const allocator_type &a)
  : _Tp_alloc_type( a ), storage_type()
  {
    ASSERT( std::distance( b, e ) == N );

    pointer cur = address();

    for(
      std::size_t cnt = N;
      cnt != 0;
      --cnt, ++b, ++cur
    ) {
      // redundant with the distance assert
      ASSERT( b != e );

      get_allocator().construct( cur, *b );
    }

    // redundant with the distance assert
    ASSERT( b == e );
  }

public: // unsafe mode
  struct unsafe_tag {};
  unsafe_array( unsafe_tag )
  : _Tp_alloc_type(), storage_type() {}

public: // access
  bool empty() const          { return false; }
  size_type size() const      { return N; }
  size_type max_size() const  { return N; }

public: // iterators
  iterator begin()                      { return address(); }
  iterator end()                        { return address() + N; }

  const_iterator begin() const          { return address(); }
  const_iterator end() const            { return address() + N; }

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
  reference       operator[](std::size_t n)       { return address()[n]; }
  const_reference operator[](std::size_t n) const { return address()[n]; }

  reference       at(std::size_t n)       { rangecheck(n); return operator[](n); }
  const_reference at(std::size_t n) const { rangecheck(n); return operator[](n); }

  template<std::size_t I> reference       at()       { rangecheck<I>(); return address()[I]; }
  template<std::size_t I> const_reference at() const { rangecheck<I>(); return address()[I]; }

private: // check range (may be private because it is static)
  static void rangecheck(size_type i)
  {
    if( i >= N )
      throw std::out_of_range("unsafe_array<>: index out of range");
  }
  template< std::size_t I >
  static void rangecheck()
  {
    STATIC_ASSERT( I < N );
  }
};

/*
template< class T, std::size_t N, class Alloc = std::allocator<T*> >
class ptr_unsafe_array
: public unsafe_array< T*, N, intrusive_allocator< Alloc > >
{
  typedef container::unsafe_array< T*, N, intrusive_allocator< Alloc > > super_type;

public:
  template<class A1>
  explicit
  ptr_unsafe_array( const A1 &a1 )
  : super_type( a1 ) {}

  template<class A1, class A2>
  ptr_unsafe_array( const A1 &a1, const A2 &a2 )
  : super_type( a1, a2 ) {}

  template<class A1, class A2, class A3>
  ptr_unsafe_array( const A1 &a1, const A2 &a2, const A3 &a3 )
  : super_type( a1, a2, a3 ) {}
};
*/

} // namespace container

#endif
