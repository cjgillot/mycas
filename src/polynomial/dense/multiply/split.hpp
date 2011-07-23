/*
 * split.hpp
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef POLY_DENSE_MULTIPLY_SPLIT_HPP_
#define POLY_DENSE_MULTIPLY_SPLIT_HPP_

#include "stdlib.hpp"
#include "tuple.hpp"

namespace poly {
namespace dense {
namespace multiply {

/*!
 * \namespace poly::dense::multiply::split
 *
 * Splitting functions
 * These return a (N+1)-tuple of iterators,
 * last one being the end iterator.
 */
namespace split {

//! \brief split helper structure
template<unsigned N, class It>
struct split_help {
  //! \brief actual recursive function
  static inline
  typename tuple::tuple_t<N+1, It>::type
  do_it(const It &b, const It &e, int step)
  {
    return tuple::cons( b
    , split_help<N-1,It>::do_it( b + step, e, step )
    );
  }
};

//! \brief split helper structure
template<class It>
struct split_help<0,It> {
  //! \brief actual base-case function
  static inline
  typename tuple::tuple_t<1, It>::type
  do_it(const It &, const It &e, int)
  {
    return tuple::cons(e);
  }
};

/*!
 * \brief Splitting function
 *
 * \param b,e : an iterator pair
 * \param step : the splitting step
 *
 * \return a (N+1)-tuple of iterators,
 *      last one being the end iterator.
 *
 * \invariant The distance between two adjacent iterators is less-eq step.
 */
template<unsigned N, class It>
typename tuple::tuple_t<N+1, It>::type
split(const It &b, const It &e, int step)
{
  return split_help<N, It>::do_it( b, e, step );
}

}}}} // namespace poly::dense::multiply::split

#endif /* POLY_DENSE_MULTIPLY_SPLIT_HPP_ */
