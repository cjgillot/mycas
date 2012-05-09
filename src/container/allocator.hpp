#ifndef CONTAINER_ALLOCATOR_HPP
#define CONTAINER_ALLOCATOR_HPP

#include <memory>
#include <boost/intrusive_ptr.hpp>

#include <boost/type_traits/remove_const.hpp>

namespace container {
namespace detail {

template< class Alloc, unsigned Sz = sizeof( Alloc ) >
struct alloc_swap {
  inline static void apply( Alloc &a, Alloc &b ) throw()
  {
    if( &a != &b )
      std::swap( a, b );
  }
};

// no-op if empty allocators
template< class Alloc >
struct alloc_swap< Alloc, 0 > {
  inline static void apply( Alloc&, Alloc& ) throw() {}
};

} // namespace detail

template< class T, class Alloc >
class intrusive_allocator
: private Alloc::template rebind<T>::other
{
  typedef typename Alloc::template rebind<T>::other super_type;

public:
  typedef typename super_type::size_type        size_type;
  typedef typename super_type::difference_type  difference_type;

  typedef typename super_type::const_pointer    const_pointer;
  typedef typename super_type::pointer          pointer;

  typedef typename super_type::const_reference  const_reference;
  typedef typename super_type::reference        reference;

  typedef typename super_type::value_type       value_type;

  typedef boost::true_type has_assign;
  typedef boost::true_type has_swap;

public:
  template<class U>
  struct rebind
  {
    typedef intrusive_allocator< U, Alloc > other;
  };

public:
  intrusive_allocator() {}
  intrusive_allocator( const intrusive_allocator &o )
  : super_type( o ) {}
  template<class U, class B>
  intrusive_allocator( const intrusive_allocator<U,B> &o )
  : super_type( o ) {}
  intrusive_allocator& operator=( const intrusive_allocator &o )
  {
    super_type::operator=( o );
    return *this;
  }
  template<class U, class B>
  intrusive_allocator& operator=( const intrusive_allocator<U,B> &o )
  {
    super_type::operator=( o );
    return *this;
  }

  void swap( intrusive_allocator &o )
  { detail::alloc_swap<Alloc>( *this, o ); }

  using super_type::address;
  using super_type::max_size;

  using super_type::allocate;
  using super_type::deallocate;

  void construct( pointer p, value_type x )
  {
    super_type::construct( p, x );
    intrusive_ptr_add_ref( *p );
  }

  template<typename... Args>
  void construct( pointer p, Args&& ...args )
  {
    super_type::construct( p, std::forward<Args>(args)... );
    intrusive_ptr_add_ref( *p );
  }

  void destroy( pointer p )
  {
    intrusive_ptr_release( *p );
    super_type::destroy( p );
  }

  void assign( pointer p, value_type x )
  {
    value_type y = std::move( *p );

    intrusive_ptr_add_ref( x );
    *p = std::move( x );
    intrusive_ptr_release( y );
  }
};

} // namespace container

namespace std {

template< class T, class Alloc >
void swap( container::intrusive_allocator<T,Alloc> &a,
           container::intrusive_allocator<T,Alloc> &b )
{ a.swap( b ); }

}

#endif
