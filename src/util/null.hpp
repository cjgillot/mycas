#ifndef UTIL_NULL_HPP
#define UTIL_NULL_HPP

#include<boost/config.hpp>

#ifndef BOOST_HAS_NULLPTR

namespace util {

struct nullptr_t {
  nullptr_t() throw() {}
 ~nullptr_t() throw() {}

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
};

// avoid GCC warning: identifier 'nullptr' will become a keyword in C++0x [-Wc++0x-compat]
#define nullptr nullptr_
static const nullptr_t nullptr;

}

using util::nullptr;

#endif

#endif
