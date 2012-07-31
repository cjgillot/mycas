#ifndef ANALYSIS_MEMORY_IPP
#define ANALYSIS_MEMORY_IPP

#include "analysis/basic/memory.hpp"

#include <boost/pool/pool.hpp>
// #include <boost/aligned_storage.hpp>

#include "util/assert.hpp"

// Usage of these pools improves memory allocation
// of basic-like objects by a great factor (at least 10x)

namespace analysis {
namespace __memory {
static inline unsigned get_pool(std::size_t) noexcept;

struct initializer_t {
private:
  boost::pool<> pool1;
  boost::pool<> pool2;
  boost::pool<> pool3;

public:
  inline  initializer_t() noexcept;
  inline ~initializer_t() noexcept;
  inline void touch() const noexcept {}
};

extern boost::pool<>* pools [4];
extern const initializer_t initializer;
}}

#define VP_SIZE (sizeof(void*))
// #define CEIL(a, b) (((a) + (b) - 1)/(b))

inline void*
analysis::__memory::allocate( std::size_t n, std::size_t offset ) noexcept
{
  std::size_t sz = n;

  unsigned pool_number = get_pool(sz);
  boost::pool<>* pool = pools[pool_number];

  ASSERT( !pool || pool->get_requested_size() >= sz );

  char* p = pool
  ? (char*)pool->malloc()
  : (char*)::operator new( sz, std::nothrow );

  p[offset] = pool_number;
  return (void*)p;
}

inline void
analysis::__memory::release( void* p0, std::size_t offset ) noexcept
{
  char* p = (char*)p0;
  boost::pool<>* pool = pools[ p[offset] & 0x3 ];

  if( pool )
  {
    ASSERT( pool->is_from( p ) );
    pool->free( p );
  }
  else
    ::operator delete( p );
}

static inline unsigned
analysis::__memory::get_pool( std::size_t sz ) noexcept
{
  initializer.touch();

  ASSERT( sz > VP_SIZE );

  if( sz > 16*VP_SIZE )
    return 0;

  if( sz >  8*VP_SIZE )
    return 3;

  if( sz >  4*VP_SIZE )
    return 2;

  return 1;
}

#undef VP_SIZE
#undef CEIL

extern template class boost::pool<>;

#endif
