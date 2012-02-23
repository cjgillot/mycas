#ifndef EXPAIRSEQ_POLY_IPP
#define EXPAIRSEQ_POLY_IPP

#include <boost/pool/singleton_pool.hpp>

#include "analysis/vectorseq/poly.hpp"

namespace analysis {
namespace vseq {

struct poly_pool_tag {};

template<class _Tp, class _Alloc>
void* poly<_Tp, _Alloc>::operator new( std::size_t n, std::nothrow_t ) throw()
{
  ASSERT( n == sizeof( poly ) ); (void)n;

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


}} // namespace analysis::vseq

#endif
