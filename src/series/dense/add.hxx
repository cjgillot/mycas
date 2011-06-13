/*
 * add.hxx
 *
 *  Created on: 11 juin 2011
 *      Author: k1000
 */

#ifndef SERIES_DENSE_ADD_HXX_
#define SERIES_DENSE_ADD_HXX_

namespace series {
namespace sparse {
namespace detail {

using namespace imperative::stream;

// due to flow requirements,
// null coefficients are allowed
template<class M, class FL, class FR, class F2>
class combine_iterator
: public iterator_base<M>
{
  FL fl;
  FR fr;
  F2 f2;

  boost::intrusive_ptr<iterator_base<M> >
    left, right;

public:
  inline
  combine_iterator()
  : fl(), fr(), f2()
  , left(), right() {}

  inline
  combine_iterator(
      iterator_base<M>* l, iterator_base<M>* r,
      const FL &fl_, const FR &fr_, const F2 &f2_
  )
  : fl(fl_), fr(fr_), f2(f2_)
  , left(l), right(r)
  {}

  inline virtual
  ~combine_iterator() {}

public:
  inline iterator_base<M>*
  clone() const {
    return new combine_iterator(
        left.get(), right.get(),
        fl, fr, f2
    );
  }

  inline iterator_base<M>*
  incr() {
    assert(!left->empty() && !right->empty());
    int cmp = algebra::compare(left->deref(), right->deref());
    if(cmp <= 0) left ->incr();
    if(cmp >= 0) right->incr();
    if(left ->empty()) return right.get();
    if(right->empty()) return left .get();
    return this;
  }

  inline M
  deref() const {
    assert(!left->empty() && !right->empty());
    M l = left->deref(), r = right->deref();
    return f2(l,r);
  }
};

template<class M, class FL, class FR, class F2>
inline combine_iterator<M, FL, FR, F2>*
combine(
    iterator_base<M>* a, iterator_base<M>* b,
    const FL &fl, const FR &fr, const F2 &f2
) {
  return new combine_iterator<M, FL, FR, F2>(a,b,fl,fr,f2);
}

}}} // series::sparse::detail


#endif /* SERIES_DENSE_ADD_HXX_ */
