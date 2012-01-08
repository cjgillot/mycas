#ifndef CONTAINER_ALLOCATOR_HPP
#define CONTAINER_ALLOCATOR_HPP

#include <memory>
#include <boost/intrusive_ptr.hpp>

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

template< class Alloc >
class intrusive_allocator
: private Alloc
{
  typedef Alloc super_type;

public:
  typedef typename super_type::value_type       value_type;

  typedef typename super_type::const_pointer    const_pointer;
  typedef typename super_type::pointer          pointer;

  typedef typename super_type::const_reference  const_reference;
  typedef typename super_type::reference        reference;

  typedef boost::true_type has_assign;
  typedef boost::true_type has_swap;

public:
  template<class U>
  struct rebind
  {
    typedef intrusive_allocator<
      typename Alloc::template rebind<
        typename boost::remove_const<U>::type
      >::other
    > other;
  };

public:
  intrusive_allocator() {}
  intrusive_allocator( const intrusive_allocator &o )
  : super_type( o ) {}
  intrusive_allocator& operator=( const intrusive_allocator &o )
  {
    super_type::operator=( o );
    return *this;
  }

  void swap( intrusive_allocator &o )
  { detail::alloc_swap<Alloc>( *this, o ); }

  using super_type::allocate;
  using super_type::deallocate;

  void construct( pointer p, value_type x )
  {
    super_type::construct( p, x );
    intrusive_ptr_add_ref( x );
  }

  void destroy( pointer p )
  {
    intrusive_ptr_release( *p );
    super_type::destroy( p );
  }
};

} // namespace container

namespace std {

template<class Alloc>
void swap( container::intrusive_allocator<Alloc> &a,
           container::intrusive_allocator<Alloc> &b )
{ a.swap( b ); }

}

#endif
