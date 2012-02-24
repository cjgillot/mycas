#ifndef CONTAINER_PTR_HPP
#define CONTAINER_PTR_HPP

#include <boost/iterator/iterator_adaptor.hpp>

namespace container {

namespace {
template<class T>
struct ptr_traits
{
  typedef typename T::reference reference;
};

template<class T>
struct ptr_traits<T*>
{
  typedef T& reference;
};
}

template<class Iter, class Policy>
struct ptr_proxy
: private Policy
{
private:
  typedef typename boost::iterator_value<Iter>::type value_type;
  typedef typename boost::iterator_reference<Iter>::type reference;

  reference m_ref;

private:
  ptr_proxy();

public:
  ptr_proxy(reference r, const Policy &pol)
  : Policy(pol), m_ref(r) {}

  ptr_proxy(const ptr_proxy &o)
  : Policy(o), m_ref(o.m_ref) {}
  ptr_proxy&
  operator=(const ptr_proxy &o)
  {
    this->Policy::operator=( o );
    this->Policy::assign( &m_ref, o.m_ref );
    return *this;
  }

  ptr_proxy&
  operator=(value_type v)
  {
    this->Policy::assign( &m_ref, v );
    return *this;
  }

  ~ptr_proxy() {}

public:
  typename ptr_traits<value_type>::reference
  operator*() const
  { return *m_ref; }

  value_type operator->() const
  { return  m_ref; }

  operator value_type() const
  { return  m_ref; }

//   template<class T>
//   operator T() const
//   { return  m_ptr; }

//   operator void() const {}

  void swap(ptr_proxy &o)
  {
    std::swap( m_ref, o.m_ref );
  }

//   void setup(reference p)
//   {
//     m_ptr = p;
//   }
};

template<class Iter, class Policy>
struct ptr_iterator
: boost::iterator_adaptor<
    ptr_iterator<Iter, Policy> // Derived
  , Iter // Base
  , typename boost::iterator_value<Iter>::type // Value
  , typename std::iterator_traits<Iter>::iterator_category
  , ptr_proxy<Iter, Policy> // Reference
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

  inline void
  iter_swap(const ptr_iterator &o) const
  { std::iter_swap( *super_t::base_reference(), o.super_t::base_reference() ); }

private:
  friend class boost::iterator_core_access;

  typename super_t::reference
  dereference() const
  { return typename super_t::reference( *super_t::base_reference(), *policy ); }

  const Policy* policy;
};

#define TDECL( z, n, data ) \
  typename BOOST_PP_CAT( _TArg, n )

#define ADECL( z, n, data )  \
  const BOOST_PP_CAT( _TArg, n ) &BOOST_PP_CAT( arg, n )

#define ARGS( z, n, data )  \
  BOOST_PP_CAT( arg, n )

#define CTOR( z, n, data )                  \
  template<                                 \
    BOOST_PP_ENUM_##z( n, TDECL, data )     \
  >                                         \
  data(                                     \
    BOOST_PP_ENUM_##z( n, ADECL, data )     \
  )                                         \
  : super_type(                             \
      BOOST_PP_ENUM_##z( n, ARGS, data )    \
  ) {}

#define DEFINE_PTR_CONTAINER( name, base_container )              \
template< class T, class Alloc = std::allocator<T*> >             \
class name                                                        \
: public base_container< T*, intrusive_allocator< T*, Alloc > > { \
  typedef intrusive_allocator< T*, Alloc > Policy;                \
  typedef base_container< T*, Policy > super_type;                \
public:                                                           \
  name(): super_type() {}                                         \
  template<class A1>                                              \
  explicit name( const A1 &a1 ): super_type( a1 ) {}              \
  BOOST_PP_REPEAT_FROM_TO( 2, 20, CTOR, name )                    \
public:                                                           \
  typedef ptr_iterator<typename super_type::iterator, Policy> iterator; \
  typedef typename super_type::const_iterator const_iterator;           \
  typedef ptr_iterator<typename super_type::reverse_iterator, Policy> reverse_iterator; \
  typedef typename super_type::const_reverse_iterator const_reverse_iterator;           \
  const_iterator          begin() const { return super_type::begin(); } \
  const_iterator          end()   const { return super_type::end(); }   \
  const_reverse_iterator rbegin() const { return super_type::rbegin(); }\
  const_reverse_iterator rend()   const { return super_type::rend(); }  \
  iterator                begin()       { return iterator( super_type::begin(), super_type::get_allocator() ); }  \
  iterator                end()         { return iterator( super_type::end(),   super_type::get_allocator() ); }  \
  reverse_iterator       rbegin()       { return reverse_iterator( super_type::rbegin(), super_type::get_allocator() ); } \
  reverse_iterator       rend()         { return reverse_iterator( super_type::rend(),   super_type::get_allocator() ); } \
};

DEFINE_PTR_CONTAINER( ptr_unsafe_vector, unsafe_vector )
DEFINE_PTR_CONTAINER( ptr_vector, std::vector )

#undef DEFINE_PTR_CONTAINER
#undef CTOR
#undef ARGS
#undef ADECL
#undef TDECL

} // namespace container

namespace std {

template<class Ptr, class Policy>
void swap( container::ptr_proxy<Ptr, Policy> &a, container::ptr_proxy<Ptr, Policy> &b )
{ a.swap(b); }

template<class Iter, class Policy>
void iter_swap( const container::ptr_iterator<Iter, Policy> &a, const container::ptr_iterator<Iter, Policy> &b )
{ a.iter_swap(b); }

}

#endif
