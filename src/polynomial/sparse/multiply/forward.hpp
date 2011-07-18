/*
 * forward.hpp
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_FORWARD_HXX_
#define SPARSE_MULTIPLY_FORWARD_HXX_

#include "stdlib.hpp"

#include "polynomial/sparse/multiply/heap.hpp"

namespace poly {
namespace sparse {
namespace multiply {

/// polynomial specific code
template<class, class> class hobj;
template<class, class> class muler;

namespace detail {

/*!
 * \struct muler_type
 * \brief typing helper class for poly::sparse::multiply::muler
 */
template<class P, class Mul>
struct muler_type {
  //! \brief Monomial type
  typedef typename boost::range_value<P>::type mono;

  //! \brief Iterator type
  typedef typename boost::range_iterator<const P>::type iter;

  //! \brief Heap object type
  typedef util::ptr<hobj<P, Mul> > hp;

  //! \brief Multiplication heap type (see multiply/heap.hpp)
  typedef heap<
    typename boost::range_value<P>::type
  , util::ptr<hobj<P, Mul> >
  , muler<P, Mul>
  > type;
};

} // namespace detail

}}} // namespace poly::sparse::multiply

#endif /* SPARSE_MULTIPLY_FORWARD_HXX_ */
