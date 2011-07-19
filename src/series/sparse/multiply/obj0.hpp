/*
 * obj0.hpp
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SERIES_SPARSE_OBJ0_HPP_
#define SERIES_SPARSE_OBJ0_HPP_

#include "stdlib.hpp"
#include "utils.hpp"

#include "stream.hpp"

#include "series/sparse/multiply/hobj.hpp"

namespace series {
namespace sparse {
namespace multiply {

using streams::iterator_base;

template<class M, class Mul>
struct obj0
: public hobj<M>
, private Mul {

  typedef hobj<M> super;

private:
  const M fm; // no reference
  boost::intrusive_ptr<
    iterator_base<M>
  > g;

public:
  inline
  obj0(const M &f0, iterator_base<M> *git)
  : fm(f0), g(git)
  { recalc(); }
  inline
  ~obj0() {}

public:
  inline bool
  update(super* &) {
    g->incr();
    if(g->empty()) return false;
    recalc();
    return true;
  }

private:
  inline void
  recalc() {
    super::cur = Mul::operator()(fm, g->deref());
  }
};

namespace detail {

template<class M, class Mul>
inline obj0<M, Mul>*
make_obj0(const M &f0, iterator_base<M>* git) {
  if(algebra::null(f0) || !git) return 0;
  return new obj0<M,Mul>(f0, git);
}

} // namespace detail

template<class M>
inline hobj<M>*
obj0_left (const M &f0, iterator_base<M> *git) {
  typedef functor::multiplies<M> Mul;
  return detail::make_obj0<M, Mul>(f0, git);
}
template<class M>
inline hobj<M>*
obj0_right(iterator_base<M> *fit, const M &g0) {
  typedef functor::multiplies_rev<M> Mul;
  return detail::make_obj0<M, Mul>(g0, fit);
}

}}} // namespace series::sparse::multiply

#endif /* SERIES_SPARSE_OBJ0_HPP_ */
