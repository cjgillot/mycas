/*
 * utils.hxx
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

#ifndef UTILS_HXX_
#define UTILS_HXX_

#include "stdlib.hxx"

namespace algebra {

template<class S>
inline S
zero()
{ return S::zero; }

template<class S>
inline S
one()
{ return S::one; }

template<class S>
inline bool
null(const S &o)
{ return o.null(); }

template<class S>
inline bool
unit(const S &o)
{ return o.unit(); }

template<class S>
inline int
compare(const S &a, const S &b)
{ return S::compare(a,b); }

template<class S>
inline int
compare(const S *a, const S *b)
{ return algebra::compare(*a, *b); }

template<class S>
inline int
compare(S *a, S *b)
{ return algebra::compare(*a, *b); }

template<class S>
inline S &
ineg(S &o)
{ return o.ineg(); }

template<class T, class B = ::boost::detail::empty_base<T> >
struct ordered
: boost::ordered<T, B> {
  friend inline bool
  operator< (const T &a, const T &b) {
    return algebra::compare(a,b) < 0;
  }
  friend inline bool
  operator==(const T &a, const T &b) {
    return algebra::compare(a,b) == 0;
  }
};

}

#endif /* UTILS_HXX_ */
