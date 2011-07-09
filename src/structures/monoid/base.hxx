/*
 * base.hxx
 *
 *  Created on: 30 juin 2011
 *      Author: k1000
 */

#ifndef MONOID_BASE_HXX_
#define MONOID_BASE_HXX_

#include "stdlib.hxx"
#include "operators.hxx"

#include "structures/magma/data.hxx"

#include "algebra/compare.hxx"

namespace structures {
namespace monoid {

template<class T>
class base
: public operators::ordered<base<T>
, boost::multipliable2<base<T>, unsigned
> > {

  std::list<T> impl;

protected:
  inline base() {}

public:
  inline
  base(const base &o)
  : impl(o.impl) {}
  inline base&
  operator=(const base &o) {
    impl = o.impl;
    return *this;
  }
  inline void
  swap(base &o) {
    impl.swap(o.impl);
  }

public:
  inline explicit
  base(const T &l)
  : impl(1, l) {}

  inline
  ~base() {}

public:
  typedef typename std::list<T>::iterator iterator;
  typedef typename std::list<T>::const_iterator const_iterator;

  iterator begin() { return impl.begin(); }
  iterator end  () { return impl.end(); }

  const_iterator begin() const { return impl.begin(); }
  const_iterator end  () const { return impl.end  (); }

public:
  inline bool
  empty() const
  { return impl.empty(); }

private:
  static inline void
  do_add(base &a, base &b) {
    a.impl.splice(a.impl.end(), b.impl);
  }

public:
  // C = |a| + |b|
  friend inline base
  operator+(base a, const base &b)
  { return a += b; }
  // C = |b|
  inline base&
  operator+=(base o) {
    do_add(*this, o);
    return *this;
  }

  // C = n * |this|
  inline base&
  operator*=(unsigned n) {
    base cst;
    while(n >= 2) {
      if(n & 1) cst += *this;
      *this += *this;
      n >>= 1;
    }
    if(n == 0) return *this  = cst;
    if(n == 1) return *this += cst;
  }

public:
  static int
  compare(const base &a, const base &b)
  { return algebra::range_compare(a.impl, b.impl); }

private:
  template<class F>
  struct map2_type {
    typedef typename boost::result_of<F(const T&)>::type type;
  };
  template<class F>
  struct map_type {
    typedef typename boost::result_of<F(const T&)>::type reference;
    typedef typename boost::remove_reference<reference>::type value_type;
    typedef base<value_type> type;
  };

public:
  template<class F>
  inline typename map2_type<F>::type
  map2(F f) const {
    typedef typename map2_type<F>::type base2;
    base2 ret;
    foreach(const T &m, impl) {
      base2 im = f(m);
      ret.impl.splice(ret.impl.end(), im);
    }
    return ret;
  }
  template<class F>
  inline typename map_type<F>::type
  map(F f) const {
    typedef typename map_type<F>::type base2;
    base2 ret;
    boost::transform(impl, std::back_inserter(ret.impl), f);
    return ret;
  }

public:
  inline base
  reverse() const {
    base ret(*this);
    ret.impl.reverse();
    return ret;
  }

  template<class F>
  inline typename map2_type<F>::type
  rev_map2(F f) const {
    typedef typename map2_type<F>::type base2;
    base2 ret;
    reverse_foreach(const T &m, impl) {
      base2 im = f(m);
      ret.impl.splice(ret.impl.end(), im);
    }
    return ret;
  }
  template<class F>
  inline typename map_type<F>::type
  rev_map(F f) const {
    typedef typename map_type<F>::type base2;
    base2 ret;
    boost::transform(impl, std::front_inserter(ret.impl), f);
    return ret;
  }
};


}} // namespace structures::monoid


#endif /* MONOID_BASE_HXX_ */
