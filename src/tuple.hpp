/*
 * tuple.hpp
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef TUPLE_HPP_
#define TUPLE_HPP_

#include <boost/fusion/include/cons.hpp>

/*!
 * \namespace tuple
 * \brief Homogeneous tuple implementation
 *
 * This namespace gives an implementation of
 * homogeneous N-ary tuples, with functions :
 * - cons and cons_ref making tuples
 * - tuple(const T&, ...) makers
 * - tie(T&, ...) makers
 *   (up to MAX_TUPLE_ARITY)
 */
namespace tuple {

/*!
 * \brief Homogeneous tuple type
 */
template<unsigned N, class T>
class tuple_t {
  typedef typename tuple_t<N-1, T>::type rec_t;

public:
  //! \brief Actual tuple type
  typedef boost::fusion::cons<T, rec_t> type;
};

/*!
 * \brief Homogeneous tuple type - empty case
 */
template<class T>
struct tuple_t<0,T> {
  //! \brief Actual tuple type
  typedef boost::fusion::nil type;
};

//! \brief Empty tuple type
typedef boost::fusion::nil nil_t;
//! \brief Empty tuple value
static nil_t nil;

//! \brief Tuple cons'ing
template<class H, class T>
boost::fusion::cons<H, T>
cons(const H &h, const T &t) {
  return boost::fusion::cons<H,T>(h,t);
}
//! \brief 1-ary tuple creation
template<class H>
boost::fusion::cons<H>
cons(const H &h) {
  return boost::fusion::cons<H>(h);
}


//! \brief Reference tuple cons'ing
template<class H, class T>
boost::fusion::cons<H&,T>
cons_ref(H &h, const T &t) {
  return boost::fusion::cons<H&,T>(h,t);
}
//! \brief 1-ary reference tuple creation
template<class H>
boost::fusion::cons<H&>
cons_ref(H &h) {
  return boost::fusion::cons<H&>(h);
}

} // namespace tuple

#include "tuple.ipp"

#endif /* TUPLE_HPP_ */
