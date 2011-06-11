/*
 * operators.hxx
 *
 *  Created on: 11 juin 2011
 *      Author: k1000
 */

#ifndef OPERATORS_HXX_
#define OPERATORS_HXX_

#include "stdlib.hxx"

namespace operators {

template<class S>
inline int
compare(const S &a, const S &b)
{ return S::compare(a,b); }

template<class S>
inline int
compare(const S *a, const S *b)
{ return operators::compare(*a, *b); }

template<class S>
inline int
compare(S *a, S *b)
{ return operators::compare(*a, *b); }

template<class T, class B = ::boost::detail::empty_base<T> >
struct ordered
: boost::totally_ordered1<T, B> {
  friend inline bool
  operator< (const T &a, const T &b) {
    return operators::compare(a,b) < 0;
  }
  friend inline bool
  operator==(const T &a, const T &b) {
    return operators::compare(a,b) == 0;
  }
};

template<class T, class B = ::boost::detail::empty_base<T> >
struct printable
: B {
  template<class Char, class Traits>
  friend inline std::basic_ostream<Char, Traits> &
  operator<<(std::basic_ostream<Char, Traits> &ios, const T &a) {
    a.print(ios);
    return ios;
  }
};

} // namespace operators

template<class T, class Char, class Traits>
inline std::basic_ostream<Char, Traits> &
operator<<(std::basic_ostream<Char, Traits> &ios, T* a) {
  return ios << *a;
}
/*
template<class T, class Char, class Traits>
inline std::basic_ostream<Char, Traits> &
operator<<(
    std::basic_ostream<Char, Traits> &ios,
    const std::list<T> &l
) {
  ios << "std::list[ ";
  foreach(const T &e, l)
    ios << e << " ; ";
  ios << " ] ";
  return ios;
}
*/
#endif /* OPERATORS_HXX_ */
