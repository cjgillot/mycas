/*
 * tuple.hxx
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef TUPLE_HXX_
#define TUPLE_HXX_

#include <boost/fusion/include/cons.hpp>

namespace tuple {

template<unsigned N, class T>
struct tuple_t {
  typedef typename tuple_t<N-1, T>::type rec_t;
  typedef boost::fusion::cons<T, rec_t> type;
};

template<class T>
struct tuple_t<0,T> {
  typedef boost::fusion::nil type;
};

typedef boost::fusion::nil nil_t;
nil_t nil;

template<class H, class T>
boost::fusion::cons<H, T>
cons(const H &h, const T &t) {
  return boost::fusion::cons<H,T>(h,t);
}
template<class H>
boost::fusion::cons<H>
cons(const H &h, nil_t) {
  return boost::fusion::cons<H>(h);
}
template<class H>
boost::fusion::cons<H>
cons(const H &h) {
  return boost::fusion::cons<H>(h);
}

template<class H, class T>
boost::fusion::cons<H&,T>
cons_ref(H &h, const T &t) {
  return boost::fusion::cons<H&,T>(h,t);
}
template<class H>
boost::fusion::cons<H&>
cons_ref(H &h, nil_t) {
  return boost::fusion::cons<H&>(h);
}
template<class H>
boost::fusion::cons<H&>
cons_ref(H &h) {
  return boost::fusion::cons<H&>(h);
}

} // namespace tuple

#include "tuple.ixx"

#endif /* TUPLE_HXX_ */
