#include "analysis/basic/memory.hpp"

#include <boost/pool/pool.hpp>
// #include <boost/aligned_storage.hpp>

#include "util/assert.hpp"

#define VP_SIZE (sizeof(void*))

// aggregate initializer
boost::pool<>* analysis::__memory::pools [4] = { nullptr, nullptr, nullptr, nullptr };
const analysis::__memory::initializer_t analysis::__memory::initializer;

inline
analysis::__memory::initializer_t::initializer_t() noexcept
: pool1(  4 * VP_SIZE )
, pool2(  8 * VP_SIZE )
, pool3( 16 * VP_SIZE )
{
  pools[0] = nullptr;
  pools[1] = &pool1;
  pools[2] = &pool2;
  pools[3] = &pool3;
}
inline
analysis::__memory::initializer_t::~initializer_t() noexcept
{
  pools[0] =
  pools[1] =
  pools[2] =
  pools[3] =
    nullptr;
}

template class boost::pool<>;
