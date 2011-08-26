#include<boost/config.hpp>

#if !defined( BOOST_HAS_NULLPTR ) && !defined( nullptr )

// avoid GCC warning: identifier 'nullptr' will become a keyword in C++0x [-Wc++0x-compat]
#define nullptr nullptr_

namespace util {

struct nullptr_t {
  template<typename T>
  operator T*() const throw()
  { return 0; }
  template<typename T>
  operator const T*() const throw()
  { return 0; }

  template<class C, typename T>
  operator T C::* () const throw()
  { return 0; }
  template<class C, typename T>
  operator const T C::* () const throw()
  { return 0; }

private: // non-template version for safe-bool
  struct safe_bool_class {};
  typedef void(safe_bool_class::* bool_type)();

public:
  operator bool_type() const
  { return 0; }
  bool operator!() const
  { return true; }
};

static const nullptr_t nullptr = {};

}

using util::nullptr;

#endif
