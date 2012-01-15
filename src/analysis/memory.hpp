#ifndef ANALYSIS_MEMORY_HPP
#define ANALYSIS_MEMORY_HPP

#include <new> // for std::bad_alloc and std::nothrow_t

namespace analysis {

namespace memory {
  // for private use only
  void* allocate( std::size_t ) throw();
  void  release ( void* )       throw();
}

} // namespace analysis

#define REGISTER_MEMORY()                                             \
  static void operator delete( void* p, std::nothrow_t ) throw()      \
  { analysis::memory::release( p ); }                                 \
  static void operator delete( void* p ) throw()                      \
  { analysis::memory::release( p ); }                                 \
  static void* operator new( std::size_t n, std::nothrow_t ) throw()  \
  { return analysis::memory::allocate( n ); }                         \
  static void* operator new( std::size_t n ) throw( std::bad_alloc )  \
  {                                                                   \
    void* p = analysis::memory::allocate( n );                        \
    if( p ) return p;                                                 \
    throw std::bad_alloc();                                           \
  }                                                                   \
private: /* disallow array operators */                               \
  static void* operator new   []( std::size_t ) throw();              \
  static void  operator delete[]( void* )       throw();

#endif
