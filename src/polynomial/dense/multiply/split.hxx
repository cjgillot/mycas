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

// splitting functions
// return a (N+1)-tuple of iterators
// last one is the end iterator
namespace detail {
template<unsigned N, class It>
struct split_help {
  static inline
  typename tuple::tuple_t<N+1, It>::type
  do_it(const It &b, const It &e, int step) {
    return tuple::cons(b
    , split_help<N-1,It>::do_it(b+step, e, step)
    );
  }
};
template<class It>
struct split_help<0,It> {
  static inline
  typename tuple::tuple_t<1, It>::type
  do_it(const It &, const It &e, int) {
    return tuple::cons(e);
  }
};
} // namespace detail

template<unsigned N, class Range>
typename tuple::tuple_t<N+1,
  typename boost::range_iterator<Range>::type
>::type
split(Range &r, int step) {
  typedef typename boost::range_iterator<Range>::type It;
  return detail::split_help<N, It>::do_it(
    boost::begin(r)
  , boost::end(r)
  , step
  );
}


}}} // namespace poly::dense::multiply::detail

#endif /* POLY_DENSE_MULTIPLY_SPLIT_HXX_ */
