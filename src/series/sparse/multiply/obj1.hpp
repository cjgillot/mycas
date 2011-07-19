/*
 * obj1.hpp
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SERIES_SPARSE_OBJ1_HPP_
#define SERIES_SPARSE_OBJ1_HPP_

#include "stdlib.hpp"
#include "utils.hpp"

#include "stream.hpp"

#include "series/sparse/multiply/hobj.hpp"
#include "series/sparse/multiply/obj0.hpp"

namespace series {
namespace sparse {
namespace multiply {

using streams::iterator_base;

template<class M>
struct obj1
: public hobj<M> {

  typedef hobj<M> super;

private:
  boost::intrusive_ptr<
    iterator_base<M>
  > f, g2;
  boost::intrusive_ptr<
    iterator_base<M> const
  > const g;

  M f0;
  const M g0; // no references here

public:
  inline
  obj1(iterator_base<M> *fit, iterator_base<M> *git)
  : f(fit), g2(0), g(git)
  , f0(fit->deref()), g0(git->deref()) {
    super::cur = f0 * g0;
  }
  inline
  ~obj1() {}

public:
  inline bool
  first_time(super* &o) {
    assert(! g2);
    g2 = g->clone();
    g2->incr();

    if(!g2->empty()) return true;

    // g is monomial -> promote [this] to obj0 (right form)
    f->incr(); // cur=f0*g0 already output
    o=obj0_right<M>(f.get(), g0);
    return false;
  }

  virtual bool
  update(super* &o) {
    if(!g2 && !first_time(o))
      return false;

    o=obj0_left <M>(f0, g2->clone());

    f->incr();

    if(f->empty())
      return false;

    f0 = f->deref();
    super::cur = f0 * g0;

    return true;
  }
};

}}} // namespace series::sparse::multiply

#endif /* SERIES_SPARSE_OBJ1_HPP_ */
