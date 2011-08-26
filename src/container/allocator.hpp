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
    if( a != b )
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
: public Alloc
{
  typedef Alloc super_type;

public:
  typedef typename super_type::value_type value_type;
  typedef typename super_type::pointer pointer;

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

  void construct( pointer p, value_type x )
  {
    super_type::construct( p, x );
    intrusive_ptr_add_ref( x );
  }

  void release( pointer p )
  {
    super_type::destroy( p );
    intrusive_ptr_release( p );
  }
};

} // namespace container

#endif
