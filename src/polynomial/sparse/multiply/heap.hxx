/*
 * heap.hxx
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_HEAP_HXX_
#define SPARSE_MULTIPLY_HEAP_HXX_

#include "stdlib.hxx"
#include "imperative/heap.hxx"

namespace poly {
namespace sparse {
namespace multiply {

/// common implementation
/// used for both polynomials and series
template<class M, class HO, class Impl>
class heap
: private imperative::heap::chain<HO>
{
  typedef imperative::heap::chain<HO> super;

  M cur;

  typedef typename super::list_t list_t;

private:
  heap();

public:
  explicit inline
  heap(size_t n)
  : super(n), cur()
  {}
  inline
  ~heap() {
    while(!empty()) {
      boost::for_each(
        super::find_maxs()
      , std::mem_fun_ref(&HO::del)
      );
      super::delete_maxs();
    }
  }

public:
  inline
  heap(const heap &o)
  : super(o), cur(o.cur) {}
  inline heap &
  operator=(const heap &o) {
    super::operator=(o);
    cur=o.cur;
    return *this;
  }

  inline void
  swap(heap &o) {
    super::swap(o);
    std::swap(cur, o.cur);
  }

public:
  using super::empty;

  inline const M &
  get() const
  { return cur; }
  inline void
  next() {
    {
      list_t ml = super::find_maxs();
      super::delete_maxs();
      static_cast<Impl*>(this)->do_update(ml);
    }

    sync();
  }

protected:
  using super::insert;

  // must be called after insertions/deletions
  inline void
  sync() {
    if(super::empty()) return;
    cur = algebra::zero<M>();
    foreach(const HO &m, super::find_maxs())
      cur += m->value();
  }
};

}}} // namespace poly::sparse::multiply

#endif /* SPARSE_MULTIPLY_HEAP_HXX_ */
