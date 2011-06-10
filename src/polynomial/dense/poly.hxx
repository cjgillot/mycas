/*
 * poly.hxx
 *
 *  Created on: 9 juin 2011
 *      Author: k1000
 */

#ifndef POLY_HXX_
#define POLY_HXX_

#include "multiply.hxx"

namespace poly {
namespace dense {

template<class K>
struct poly
: boost::arithmetic1<poly<K>
, boost::multiplicative2<poly<K>, K
> > {
  typedef algebra::integer Z;

  typedef K k;
  typedef Z z;

  typedef std::vector<K> impl_t;

  typedef typename impl_t::iterator iterator;
  typedef typename impl_t::const_iterator const_iterator;

  mutable impl_t impl;

private:
  inline void
  reduce() const {
    while(! impl.empty())
      if(algebra::null(impl.front()))
        impl.pop_front();
      else
        return;
  }

public:
  inline
  poly() {}
  inline
  poly(const poly &o)
  : impl(o) {}
  inline poly &
  operator=(const poly &o) {
    impl = o.impl;
    return *this;
  }

public:
  inline explicit
  poly(const k &c)
  : impl(1,c) {}

  inline
  ~poly() {}

public:
  // range concept
  inline iterator
  begin()
  { return impl.begin(); }
  inline const_iterator
  begin() const
  { return impl.begin(); }

  inline iterator
  end()
  { return impl.end(); }
  inline const_iterator
  end() const
  { return impl.end(); }

public:
  static poly zero;
  static poly one;

  inline bool
  null() const {
    reduce();
    return impl::empty();
  }
  inline bool
  unit() const {
    reduce();
    return impl::size() == 1
        && algebra::unit(impl::front());
  }

  inline const z &
  deg() const {
    reduce();
    return impl::size() - 1;
  }

  inline bool
  monome() const {
    reduce();
    if(impl.empty()) return false;
    iterator it = impl.begin(), end = impl.end();
    ++it;
    for(; it != end; ++it)
      if(! algebra::null(*it))
        return false;
    return true;
  }

private:
  /// implementation of += and -= operators
  ///   requirement : m != 0, f1(m) != 0
  template<class Fnc1, class Fnc2>
  static inline void
  combine(poly &a, const poly &b, Fnc1 f1, Fnc2 f2) {
    iterator
      i1 = a.begin(),
      e1 = a.end();
    const_iterator
      i2 = b.begin(),
      e2 = b.end();

    {
      int sz = std::min(a.impl.size(), b.impl.size());
      a.impl.reserve(sz);
    }

    for(; i1 != e1 && i2 != e2; ++i1, ++i2)
      f2(*i1, *i2);

    /* if terms remain in o */
    for(; i2 != e2; ++i2)
      a.impl.push_back(f1(*i2));
  }

public:
  inline poly &
  operator+=(const poly &o) {
    reduce(); o.reduce();
    combine(*this, o, _1, _1 += _2);
    return *this;
  }
  inline poly &
  operator-=(const poly &o) {
    reduce(); o.reduce();
    combine(*this, o, - _1, _1 -= _2);
    return *this;
  }

  inline poly &
  ineg() {
    reduce();
    boost::for_each(*this, algebra::ineg<M>);
    return *this;
  }
  inline poly &
  operator*=(const k &o) {
    if(algebra::null(o)) {
      impl.clear();
      return *this;
    }
    reduce();
    boost::for_each(*this, _1 *= o);
    return *this;
  }
  inline poly &
  operator/=(const k &o) {
    assert(! algebra::null(o));
    reduce();
    boost::for_each(*this, _1 /= o);
    return *this;
  }
};

}} // namespace poly::dense

#endif /* POLY_HXX_ */
