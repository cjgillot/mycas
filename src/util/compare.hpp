#ifndef UTIL_COMPARE_HPP
#define UTIL_COMPARE_HPP

#include <boost/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/make_signed.hpp>

namespace util {

typedef signed int cmp_t;

namespace detail {

/*
sign = (v != 0) | -(int)((unsigned int)((int)v) >> (sizeof(int) * CHAR_BIT - 1));
// Or, for more speed but less portability:
sign = (v != 0) | (v >> (sizeof(int) * CHAR_BIT - 1));  // -1, 0, or +1
// Or, for portability, brevity, and (perhaps) speed:
sign = (v > 0) - (v < 0); // -1, 0, or +1
*/
template<class T>
inline cmp_t
sign(T x)
{
  BOOST_STATIC_ASSERT( boost::is_signed<T>::value );
  return (x > 0) - (x < 0);
}

}

/*!
 * \brief Integer comparison function
 * \return
 *   -1 if \c a  < \c b
 *    0 if \c a == \c b
 *   +1 if \c a  > \c b
 */
template<class T>
inline cmp_t
compare(T a, T b) {
  BOOST_STATIC_ASSERT( boost::is_integral<T>::value );
  typedef typename boost::make_signed<T>::type ST;
  return detail::sign( static_cast<ST>(a) - static_cast<ST>(b) );
}


}

#endif
