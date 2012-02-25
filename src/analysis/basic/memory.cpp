#include "analysis/basic/memory.hpp"

#include <boost/pool/pool.hpp>
#include <boost/aligned_storage.hpp>

#include "util/assert.hpp"
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
 * -----------------------...----
 * ^            ^
 * allocated    returned
 * pointer      pointer
 *
 * The pool* pointer tracks in which allocation pool the
 * chunk has been allocated, or nullptr if allocated by ::operator new.
 */

namespace {

union max_align {
  char c;
  short s;
  int i;
  long l;
  long long ll;
  size_t sz;

  void* p;
  max_align max_align::*mp;

  max_align (*fp)();
  max_align (max_align::*mfp)();
};

struct initializer_t
{
private:
  boost::pool<> pool1;
  boost::pool<> pool2;
  boost::pool<> pool3;

public:
  inline  initializer_t() throw();
  inline ~initializer_t() throw();
  inline void touch() const throw() {}
};

}

#define tag_size (sizeof(max_align))
#define vp_size (sizeof(void*))

#define TAG( p ) (*(void**)p)

STATIC_ASSERT( vp_size <= tag_size );

// aggregate initializer
static boost::pool<>* pools [3] = { 0, 0, 0 };
static const initializer_t initializer;

static inline boost::pool<>*
get_pool( std::size_t sz )
  throw()
{
  initializer.touch();

  ASSERT( sz > vp_size );

  sz = ( sz + vp_size - 1 ) / vp_size;
  sz = ( sz + 3 ) / 4;

  return ( sz < 4 ) ? pools[ sz - 1 ] : nullptr;
}

void*
analysis::__memory::allocate( std::size_t n )
  throw()
{
  std::size_t sz = n + tag_size;

  boost::pool<>* pool = get_pool( sz );

  ASSERT( !pool || pool->get_requested_size() >= sz );

  max_align* p = pool
  ? (max_align*)pool->malloc()
  : (max_align*)::operator new( sz, std::nothrow );

  TAG(p) = pool;
  return ++p;
}

void
analysis::__memory::release( void* p )
  throw()
{
  --*(max_align**)&p;

  if( TAG(p) )
  {
    boost::pool<>* pool = static_cast<boost::pool<>*>( TAG(p) );

    ASSERT( pool->is_from( p ) );
    pool->free( p );
  }
  else
    ::operator delete( p );
}
#undef DATA

inline
initializer_t::initializer_t()
  throw()
: pool1( 4  * sizeof( void* ) )
, pool2( 8  * sizeof( void* ) )
, pool3( 12 * sizeof( void* ) )
{
  pools[0] = &pool1;
  pools[1] = &pool2;
  pools[2] = &pool3;
}
inline
initializer_t::~initializer_t()
  throw()
{
  pools[0] =
  pools[1] =
  pools[2] =
    nullptr;
}

template class boost::pool<>;
