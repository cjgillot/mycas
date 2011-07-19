/*
 * sum.hpp
 *
 *  Created on: 11 juin 2011
 *      Author: k1000
 */

#ifndef SERIES_ADD_HPP_
#define SERIES_ADD_HPP_

#include "stream.hpp"

namespace series {
namespace sparse {
namespace detail {

using streams::iterator_base;
using streams::meta_iterator;

template<class It1, class It2, class FL, class FR, class F2>
struct combine_iterator_base {
  typedef typename boost::iterator_value<It1>::type Arg1;
  typedef typename boost::iterator_value<It2>::type Arg2;
  typedef typename boost::iterator_reference<It1>::type Arg1r;
  typedef typename boost::iterator_reference<It2>::type Arg2r;
  typedef typename boost::result_of<F2(Arg1r, Arg2r)>::type Res_t;

  typedef iterator_base<Res_t> type;
};

template<class Arg1, class Arg2>
struct saving {
  Arg1 l;
  Arg2 r;
  int cmp;

public:
  inline
  saving(const saving &o)
  : l(o.l), r(o.r), cmp(o.cmp) {}
  inline saving &
  operator=(const saving &o) {
    l=o.l; r=o.r; cmp=o.cmp;
    return *this;
  }

public:
  inline
  saving(const Arg1 &l, const Arg2 &r, int c)
  : l(l), r(r), cmp(c) {}

  inline
  ~saving() {}

  inline void
  swap(saving &o) {
    std::swap(l,o.l);
    std::swap(r,o.r);
    std::swap(cmp,o.cmp);
  }

private:
  saving();
};

// due to flow requirements,
// null coefficients are allowed
template<class It1, class It2, class FL, class FR, class F2>
class combine_iterator
: public combine_iterator_base<It1, It2, FL,FR,F2>::type {
  FL fl;
  FR fr;
  F2 f2;

  typedef combine_iterator_base<It1, It2, FL,FR,F2> types;

  typedef typename types::Arg1  Arg1;
  typedef typename types::Arg2  Arg2;
  typedef typename types::Res_t Res_t;

  It1 left;
  It2 right;

  typedef saving<Arg1, Arg2> save_t;
  mutable boost::optional<save_t> saved;

private:
  combine_iterator();

public:
  inline
  combine_iterator(const combine_iterator &o)
  : fl(o.fl), fr(o.fr), f2(o.f2)
  , left(o.left), right(o.right)
  , saved(o.saved) {}
  inline combine_iterator&
  operator=(const combine_iterator &o) {
    fl=o.fl; fr=o.fr; f2=o.f2;
    left=o.left; right=o.right;
    saved=o.saved;
    return *this;
  }

  inline void
  swap(combine_iterator &o) {
    std::swap(fl,o.fl); std::swap(fr,o.fr);
    std::swap(f2,o.f2);
    std::swap(left,o.left);
    std::swap(right,o.right);
    std::swap(saved,o.saved);
  }

  inline
  combine_iterator(
      const It1 l, const It2 &r,
      const FL &fl_, const FR &fr_, const F2 &f2_
  )
  : fl(fl_), fr(fr_), f2(f2_)
  , left(l), right(r) {}

  inline
  ~combine_iterator() {}

private:
  inline void
  save() const {
    if(saved) return;
    Arg1 l = *left;
    Arg2 r = *right;
    int cmp = algebra::compare(l,r);
    saved=save_t(l,r,cmp);
  }
  inline void
  unsave() const {
    saved=boost::none;
  }

public:
  inline iterator_base<Res_t>*
  clone() const
  { return new combine_iterator(*this); }

  inline iterator_base<Res_t>*
  incr() {
    assert(left && right);

    save();

    int cmp = saved->cmp;

    if(cmp <= 0) ++left;
    if(cmp >= 0) ++right;

    if(!left ) return streams::map_iter(right, fr);
    if(!right) return streams::map_iter(left , fl);

    unsave();

    return this;
  }

  inline Res_t
  deref() const {
    assert(left && right);

    save();

    int cmp = saved->cmp;

    if(cmp < 0)      return fl(saved->l);
    else if(cmp > 0) return fr(saved->r);
    else             return f2(saved->l,saved->r);
  }
};


template<class It1, class It2, class FL, class FR, class F2>
inline combine_iterator<It1, It2, FL, FR, F2>*
combine(
    const It1 &a, const It2 &b,
    const FL &fl, const FR &fr, const F2 &f2
) {
  return new combine_iterator<It1, It2, FL, FR, F2>(a,b,fl,fr,f2);
}

}}} // series::sparse::detail


#endif /* SERIES_ADD_HPP_ */
