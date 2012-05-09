#ifndef ANALYSIS_MEMORY_HPP
#define ANALYSIS_MEMORY_HPP

#include <new> // for std::bad_alloc and std::nothrow_t

namespace analysis {

namespace __memory {
  // for private use only
  void* allocate( std::size_t ) noexcept;
  void  release ( void* )       noexcept;
}

} // namespace analysis

#define REGISTER_MEMORY()                                             \
public:                                                               \
  static void operator delete( void* p, std::nothrow_t ) throw()      \
  { analysis::__memory::release( p ); }                               \
  static void operator delete( void* p ) throw()                      \
  { analysis::__memory::release( p ); }                               \
  static void* operator new( std::size_t n, std::nothrow_t ) throw()  \
  { return analysis::__memory::allocate( n ); }                       \
  static void* operator new( std::size_t n ) throw( std::bad_alloc )  \
  {                                                                   \
    void* p = analysis::__memory::allocate( n );                      \
    if( p ) return p;                                                 \
    throw std::bad_alloc();                                           \
  }                                                                   \
private: /* disallow array operators */                               \
  static void* operator new   []( std::size_t ) throw();              \
  static void  operator delete[]( void* )       throw();

#endif
