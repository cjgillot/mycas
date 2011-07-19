/*
 * muler.hpp
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SERIES_SPARSE_MULER_HPP_
#define SERIES_SPARSE_MULER_HPP_

#include "stdlib.hpp"
#include "utils.hpp"

#include "stream.hpp"

#include "polynomial/sparse/multiply/heap.hpp"

#include "series/sparse/multiply/hobj.hpp"
#include "series/sparse/multiply/obj1.hpp"

namespace series {
namespace sparse {
namespace multiply {

using streams::iterator_base;
using poly::sparse::multiply::heap;

template<class M>
class muler
: private heap<
    M
  , util::ptr<hobj<M> >
  , muler<M>
  > {

  typedef hobj<M> ho_t;
  typedef heap<M, util::ptr<ho_t>, muler> super;

protected:
  friend class heap<M, util::ptr<ho_t>, muler>;
  template<class Range>
  inline void
  do_update(const Range &ml) {
    foreach(util::ptr<ho_t> m, ml) {
      ho_t* o = 0;

      if(m->update(o)) super::insert(m);
      else delete m.get();

      if(o)
        super::insert(o);
    }
  }

private:
  muler();

public:
  inline
  muler(iterator_base<M> *a, iterator_base<M> *b)
  : super(1) // initial size = 1 -> profiling ?
  {
    assert(!a->empty() && !b->empty());
    super::insert(new obj1<M>(a, b));
    super::sync();
  }
  inline
  ~muler() {} // super cares for deleting all the hobjs

public:
  inline
  muler(const muler &o)
  : super(o) {}
  inline muler&
  operator=(const muler &o) {
    super::operator=(o);
    return *this;
  }

  inline void
  swap(muler &o) {
    super::swap(o);
  }

public:
  using super::empty;
  using super::get;
  using super::next;
};

}}} // namespace series::sparse::multiply

#endif /* SERIES_SPARSE_MULER_HPP_ */
