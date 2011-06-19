/*
 * split.hxx
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef POLY_DENSE_MULTIPLY_SPLIT_HXX_
#define POLY_DENSE_MULTIPLY_SPLIT_HXX_

#include "stdlib.hxx"
#include "tuple.hxx"

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

/*!
 * \brief split helper function
 */
template<unsigned N, class It>
struct split_help {
  /*!
   * \brief actual function
   */
  static inline
  typename tuple::tuple_t<N+1, It>::type
  do_it(const It &b, const It &e, int step) {
    return tuple::cons(b
    , split_help<N-1,It>::do_it(b+step, e, step)
    );
  }
};

/*!
 * \brief split helper function
 */
template<class It>
struct split_help<0,It> {
  /*!
   * \brief actual function
   */
  static inline
  typename tuple::tuple_t<1, It>::type
  do_it(const It &, const It &e, int) {
    return tuple::cons(e);
  }
};

/*!
 * \fn tuple<N, It> split (Range &r, int step)
 *
 * \brief Splitting function
 *
 * \param r : the range to be split
 * \param step : the splitting step
 *
 * \return a (N+1)-tuple of iterators,
 *      last one being the end iterator.
 */
template<unsigned N, class Range>
typename tuple::tuple_t<N+1,
  typename boost::range_iterator<Range>::type
>::type
split(Range &r, int step) {
  typedef typename boost::range_iterator<Range>::type It;
  return split_help<N, It>::do_it(
    boost::begin(r)
  , boost::end(r)
  , step
  );
}

}}}} // namespace poly::dense::multiply::split

#endif /* POLY_DENSE_MULTIPLY_SPLIT_HXX_ */
