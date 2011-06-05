/*
 * series.hxx
 *
 *  Created on: 5 juin 2011
 *      Author: k1000
 */

#ifndef SERIES_HXX_
#define SERIES_HXX_

namespace poly {
namespace sparse {

// for now, series are infinite

// shall we consider some finite ones ?
// -> faster operations with consumed finite
// -> slower operations with infinite (or un-consumed finite)

namespace series_detail {

// due to flow requirements,
// null coefficients are allowed
template<class It1, class It2, class FL, class FR, class F2>
class combine_iterator
: public boost::iterator_facade
    < combine_iterator<It1, It2, FL, FR, F2>
    , typename boost::iterator_value<It1>::type const
    , boost::forward_traversal_tag
    >
, boost::noncopyable
{
  typedef typename boost::iterator_value<It1>::type value_type;

  typedef typename It1::mono mono;

  FL fl;
  FR fr;
  F2 f2;

  It1 left;
  It2 right;

  mutable value_type cur;
  mutable bool ready;

public:
  inline
  combine_iterator()
  : fl(), fr(), f2()
  , left(), right() {}
  inline
  combine_iterator(const combine_iterator &o)
  : fl(o.fl), fr(o.fr), f2(o.f2)
  , left(o.left), right(o.right)
  {}
  inline combine_iterator &
  operator=(const combine_iterator &o) {
    fl = o.fl; fr = o.fr; f2 = o.f2;
    left = o.left; right = o.right;
    return *this;
  }

  inline
  combine_iterator(const It1 &l, const It2 &r, FL fl_, FR fr_, F2 f2_)
  : left(l), right(r)
  , fl(fl_), fr(fr_), f2(f2_)
  {}

  inline
  ~combine_iterator() {}

private:
  friend class boost::iterator_core_access;

  inline void
  increment() {
    int cmp = algebra::compare(*left, *right);
    if(cmp <= 0) ++left;
    if(cmp >= 0) ++right;
  }

  inline bool
  equal(const combine_iterator &other) const {
    return this->left  == other.left
        && this->right == other.right
        && this->f1    == other.f1
        && this->f2    == other.f2;
  }

  inline const value_type &
  dereference() const {
     if(ready) return cur;

     int cmp = algebra::compare(*left, *right);
     cur = algebra::zero<value_type>();

          if(cmp < 0) cur = fl(*left);
     else if(cmp > 0) cur = fr(*right);
     else cur = f2(*left, *right);

     ready = true;

     return cur;
   }
};

template<class It1, class It2, class FL, class FR, class F2>
combine_iterator<It1, It2, FL, FR, F2>
combine(const It1 &a, const It2 &b, FL fl, FR fr, F2 f2) {
  return combine_iterator<It1, It2, FL, FR, F2>(a,b,fl,fr,f2);
}

template<class It1, class It2, class FL, class FR, class F2>
combine_iterator<It1, It2, FL, FR, F2>
add(const It1 &a, const It2 &b) {
  return combine(a,b, _1, _1, _1 + _2);
}

template<class It1, class It2, class FL, class FR, class F2>
combine_iterator<It1, It2, FL, FR, F2>
sub(const It1 &a, const It2 &b) {
  return combine(a,b, _1, -_1, _1 - _2);
}


// due to flow requirements,
// null coefficients are allowed
template<class It1, class It2>
class mul_iterator
: public boost::iterator_facade
    < mul_iterator<It1, It2>
    , typename boost::iterator_value<It1>::type const
    , boost::forward_traversal_tag
    >
, boost::noncopyable
{
  typedef typename boost::iterator_value<It1>::type value_type;

  typedef typename It1::mono mono;

  multiply::series::series<It1, It2> impl;

public:
  inline
  mul_iterator()
  : impl() {}
  inline
  mul_iterator(const mul_iterator &o)
  : impl(o.impl)
  {}
  inline mul_iterator &
  operator=(const mul_iterator &o) {
    impl = o.impl;
    return *this;
  }

  inline
  mul_iterator(const It1 &l, const It2 &r)
  : impl(l,r)
  {}

  inline
  ~mul_iterator() {}

private:
  friend class boost::iterator_core_access;

  inline void
  increment()
  { impl.next(); }

  inline bool
  equal(const mul_iterator &other) const
  { return false; } //this->impl == other.impl; }

  inline const value_type &
  dereference() const
  { return impl.get(); }
};

template<class It1, class It2>
mul_iterator<It1, It2>
mul(const It1 &a, const It2 &b) {
  return mul_iterator<It1,It2>(a,b);
}


}

}} // poly::sparse


#endif /* SERIES_HXX_ */
