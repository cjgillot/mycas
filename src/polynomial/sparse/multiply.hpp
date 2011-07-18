/*
 * multiply.hpp
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_HXX
#define SPARSE_MULTIPLY_HXX

#include "stdlib.hpp"

#include "polynomial/sparse/multiply/heap.hpp"
#include "polynomial/sparse/multiply/forward.hpp"
#include "polynomial/sparse/multiply/hobj.hpp"
#include "polynomial/sparse/multiply/muler.hpp"

namespace poly {
namespace sparse {
namespace multiply {

namespace detail {

template<class Mono, class Muler, class Range1, class RRange>
inline void
do_mul_loop(const Range1 &a, const Range1 &b, RRange &r) {
  for(Muler muler(a,b);
      ! muler.empty();
      muler.next()) {
    const Mono &m = muler.get();

    if(!algebra::null(m))
      r.push_back(m);
  }
}


template<class Mono, class Range1, class RRange>
inline void
do_mul_left (const Range1 &a, const Range1 &b, RRange &r) {
  typedef functor::multiplies<Mono> mul;
  typedef muler<Range1, mul> muler;

  do_mul_loop<Mono, muler, Range1, RRange>(a,b,r);
}
template<class Mono, class Range1, class RRange>
inline void
do_mul_right(const Range1 &a, const Range1 &b, RRange &r) {
  typedef functor::multiplies_rev<Mono> mul;
  typedef muler<Range1, mul> muler;

  do_mul_loop<Mono, muler, Range1, RRange>(b,a,r);
}

} // namespace detail


/*!
 * \brief Dispatch function for sparse multiplication
 *
 * This function chooses the major polynomial
 * (the shortest), being the first argument of muler.
 * Here comes the {Mul} template argument in {muler} and
 * {hobj} : it says whether the product is right {a * b} or
 * reversed {b *' a} with *' the reversed operator.
 * See {detail::do_mul_left} and {detail::do_mul_right} for {Mul} coping.
 * See {detail::do_mul_loop} for muler usage.
 *
 * \param a,b : two polynomials
 * \param r : the return range
 */
template<class Mono, class Range1, class RRange>
inline void
do_mul(const Range1 &a, const Range1 &b, RRange &r) {
  assert(!a.empty() && !b.empty());

  // space size is linear in the length of the first poly
  if(a.size() < b.size())
    detail::do_mul_left <Mono, Range1, RRange>(a,b,r);
  else
    detail::do_mul_right<Mono, Range1, RRange>(a,b,r);
}

}}} // poly::sparse::multiply

#endif // SPARSE_MULTIPLY_HXX
