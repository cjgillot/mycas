#include "analysis/register.hpp"

#include <boost/pool/pool.hpp>
#include <boost/aligned_storage.hpp>

#include "analysis/sum.hpp"
#include "analysis/prod.hpp"
#include "analysis/power.hpp"

#include "util/null.hpp"

// Usage of these pools improves memory allocation
// of basic-like objects by a great factor (at least 10x)

/*
 * Allocation policy :
 *
 * When allocate is called for a size n,
 * return a pointer p to a block of size (n+1)
 * with following layout :
 * ---------------------- ... ---
 * | pool*      | object  ...   |
 * ------------------------------
 * ^            ^
 * allocated    returned
 * pointer      pointer
 *
 * The pool* pointer tracks in which allocation pool the
 * chunk has been allocated, or nullptr if allocated by ::operator new.
 */

namespace {

STATIC_ASSERT( sizeof( analysis::sum ) == sizeof( analysis::prod ) );

boost::pool<> pool1 ( 4  * sizeof( void* ) );
boost::pool<> pool2 ( 8  * sizeof( void* ) );
boost::pool<> pool3 ( 12 * sizeof( void* ) );

boost::pool<>* pools [] = {
  &pool1, &pool2, &pool3
};

boost::pool<>* get_pool( std::size_t sz )
{
  ASSERT( sz > sizeof( void* ) );
  sz = ( sz + sizeof( void* ) - 1 ) / sizeof( void* );

  sz = ( sz - 1 ) / 4;

  return ( sz < 4 ) ? pools[ sz ] : nullptr;
}

}

#define DATA( p ) ( static_cast<void**>( p ) )

namespace analysis {
namespace memory {

void* allocate( std::size_t n ) throw()
{
  std::size_t sz = n + sizeof( void* );

  boost::pool<>* pool = get_pool( sz );

  ASSERT( !pool || pool->get_requested_size() >= sz );

  void* p = pool
  ? pool->malloc()
  : ::operator new( sz, std::nothrow );

  *DATA( p ) = pool;
  return DATA( p ) + 1;
}

void release( void* p ) throw()
{
  p = DATA( p ) - 1;

  if( *DATA( p ) )
  {
    boost::pool<>* pool = *static_cast<boost::pool<>**>( p );
    ASSERT( pool->is_from( p ) );
    pool->free( p );
  }
  else
    ::operator delete( p );
}

}} // namespace analysis::memory
