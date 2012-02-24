#ifndef UTIL_COMPARE_HPP
#define UTIL_COMPARE_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/make_signed.hpp>

#include <boost/range.hpp>

#include "util/assert.hpp"

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
  STATIC_ASSERT( boost::is_signed<T>::value );
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
  STATIC_ASSERT( boost::is_integral<T>::value );
  typedef typename boost::make_signed<T>::type ST;
  return detail::sign( static_cast<ST>(a) - static_cast<ST>(b) );
}

/*!
 * \brief Pointer comparison function
 *
 * The pointers are compared, not the pointees.
 *
 * \return
 *   -1 if \c a  < \c b
 *    0 if \c a == \c b
 *   +1 if \c a  > \c b
 */
template<class T>
inline cmp_t
compare(T* a, T* b) {
  return detail::sign( a - b );
}

template<class T>
inline cmp_t
compare(const T* a, const T* b) {
  return detail::sign( a - b );
}

template<class Range, class BinOp>
inline cmp_t
compare_range(const Range &a, const Range &b, BinOp op)
{
  typedef typename boost::range_iterator<Range>::type iterator;
  iterator
    it1 = boost::begin(a), en1 = boost::end(a),
    it2 = boost::begin(b), en2 = boost::end(b);
  for( ; it1 != en1 && it2 != en2; ++it1, ++it2 )
  {
    cmp_t c = op( *it1, *it2 );
    if( c ) return c;
  }

  if( it1 != en1 ) return 1;
  if( it2 != en2 ) return-1;

  return 0;
}

}

#endif
