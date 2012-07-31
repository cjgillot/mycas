#ifndef ANALYSIS_MEMORY_HPP
#define ANALYSIS_MEMORY_HPP

#include <cstddef>
#include <new> // for std::bad_alloc and std::nothrow_t

namespace analysis {

namespace __memory {
  // for private use only
  void* allocate( std::size_t, std::size_t ) noexcept;
  void  release ( void*, std::size_t )       noexcept;

  struct pool_object {
    unsigned char alloc_data;

    pool_object() {}
    pool_object(const pool_object&) {}
    pool_object(pool_object&&) {}

    pool_object& operator=(const pool_object&) { return *this; }
    pool_object& operator=(pool_object&&) { return *this; }
  };
}

template<class Derived>
struct memory_base
: private __memory::pool_object
{
private:
  static constexpr std::size_t offset() {
    return ((std::size_t)(&static_cast<__memory::pool_object*>((Derived*)0xdeadbeef)->alloc_data - (unsigned char*)0xdeadbeef));
  }

public:
  static void operator delete( void* p, std::nothrow_t ) throw()
  { analysis::__memory::release( p, offset() ); }
  static void operator delete( void* p ) throw()
  { analysis::__memory::release( p, offset() ); }
  static void* operator new( std::size_t n, std::nothrow_t ) throw()
  { return analysis::__memory::allocate( n, offset() ); }
  static void* operator new( std::size_t n ) throw( std::bad_alloc )
  {
    void* p = analysis::__memory::allocate( n, offset() );
    if( p ) return p;
    throw std::bad_alloc();
  }

private:
#undef offset

private: /* disallow array operators */
  static void* operator new   []( std::size_t ) throw();
  static void  operator delete[]( void* )       throw();
};

} // namespace analysis

#if 0
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

#ifndef ANALYSIS_MEMORY_CPP
#include "analysis/basic/memory.ipp"
#endif

#endif
