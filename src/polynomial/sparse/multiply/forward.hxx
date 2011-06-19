/*
 * forward.hxx
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_FORWARD_HXX_
#define SPARSE_MULTIPLY_FORWARD_HXX_

#include "stdlib.hxx"

#include "polynomial/sparse/multiply/heap.hxx"

namespace poly {
namespace sparse {
namespace multiply {

/// polynomial specific code
template<class, class> class hobj;
template<class, class> class muler;

namespace detail {

template<class P, class Mul>
struct muler_type {
  typedef typename boost::range_value<P>::type mono;
  typedef typename boost::range_iterator<const P>::type iter;
  typedef util::ptr<hobj<P, Mul> > hp;
  typedef heap<
    typename boost::range_value<P>::type
  , util::ptr<hobj<P, Mul> >
  , muler<P, Mul>
  > type;
};

} // namespace detail

}}} // namespace poly::sparse::multiply

#endif /* SPARSE_MULTIPLY_FORWARD_HXX_ */
