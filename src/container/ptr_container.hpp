#ifndef CONTAINER_PTR_HPP
#define CONTAINER_PTR_HPP

#include <boost/iterator/iterator_adaptor.hpp>

namespace container {

template<class Ptr>
struct ptr_traits
{
  typedef typename Ptr::reference reference;
};

template<class T>
struct ptr_traits<T*>
{
  typedef T& reference;
};

template<class Ptr, class Policy>
struct ptr_proxy
: private Policy
{
  ptr_proxy(Ptr p, const Policy &pol)
  : Policy(pol), m_ptr(p) {}

  typename ptr_traits<Ptr>::reference
  operator*() const
  { return *m_ptr; }

  Ptr operator->() const
  { return  m_ptr; }

  operator Ptr() const
  { return  m_ptr; }

//   template<class T>
//   operator T() const
//   { return  m_ptr; }

//   operator void() const {}

  ptr_proxy&
  operator=(Ptr p)
  {
    this->Policy::assign( m_ptr, p );
    return *this;
  }

  void swap(ptr_proxy &o)
  {
    this->Policy::swap( m_ptr, o.m_ptr );
  }

  void setup(Ptr p)
  {
    m_ptr = p;
  }

private:
  Ptr m_ptr;
};

template<class Iter, class Policy>
struct ptr_iterator
: boost::iterator_adaptor<
    ptr_iterator<Iter, Policy> // Derived
  , Iter // Base
  , ptr_proxy<
      typename boost::iterator_value<Iter>::type
    , Policy
    > // Value
  , typename std::iterator_traits<Iter>::iterator_category
  , ptr_proxy<
      typename boost::iterator_value<Iter>::type
    , Policy
    > // Reference
  >
{
private:
  typedef typename ptr_iterator::iterator_adaptor_ super_t;

public:
  typedef typename super_t::value_type        value_type;
  typedef typename super_t::reference         reference;
  typedef typename super_t::pointer           pointer;
  typedef typename super_t::difference_type   difference_type;

  ptr_iterator(const Iter &i, const Policy &p)
  : super_t(i), policy(&p) {}

private:
  friend class boost::iterator_core_access;

  typename super_t::reference
  dereference() const
  { return typename super_t::reference( *super_t::base_reference(), *policy ); }

  const Policy* policy;
};

template< class T, class Alloc = std::allocator<T*> >
class ptr_unsafe_vector
: public unsafe_vector< T*, intrusive_allocator< Alloc > >
{
  typedef container::unsafe_vector< T*, intrusive_allocator< Alloc > > super_type;

public:
  template<class A1>
  explicit
  ptr_unsafe_vector( const A1 &a1 )
  : super_type( a1 ) {}

  template<class A1, class A2>
  ptr_unsafe_vector( const A1 &a1, const A2 &a2 )
  : super_type( a1, a2 ) {}

  template<class A1, class A2, class A3>
  ptr_unsafe_vector( const A1 &a1, const A2 &a2, const A3 &a3 )
  : super_type( a1, a2, a3 ) {}

public:
  typedef typename super_type::iterator       iterator;
  typedef typename super_type::iterator const_iterator;

  typedef typename super_type::const_reverse_iterator       reverse_iterator;
  typedef typename super_type::const_reverse_iterator const_reverse_iterator;

  iterator       begin()       { return super_type::begin(); }
  const_iterator begin() const { return super_type::begin(); }

  iterator       end()         { return super_type::end(); }
  const_iterator end()   const { return super_type::end(); }

  reverse_iterator       rbegin()       { return super_type::rbegin(); }
  const_reverse_iterator rbegin() const { return super_type::rbegin(); }

  reverse_iterator       rend()         { return super_type::rend(); }
  const_reverse_iterator rend()   const { return super_type::rend(); }
};

}

namespace std {

template<class Ptr, class Policy>
void swap( container::ptr_proxy<Ptr, Policy> &a, container::ptr_proxy<Ptr, Policy> &b )
{ a.swap(b); }

}

#endif
