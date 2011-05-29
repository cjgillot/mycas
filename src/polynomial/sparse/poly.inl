/*
 * poly.inl
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

#ifndef POLY_INL_
#define POLY_INL_

#include "stdlib.hxx"

#include "poly.hxx"

namespace poly {
namespace sparse {

template<class K>
inline
poly<K>::poly()
: impl(0)
{}
template<class K>
inline
poly<K>::poly(const poly &m)
: impl(m)
{}
template<class K>
inline poly<K> &
poly<K>::operator=(const poly &m) {
  impl::operator=(m);
  return *this;
}
template<class K>
inline
poly<K>::~poly()
{}


template<class K>
inline
poly<K>::poly(const k &c, const z &e)
: impl(1,M(c,e))
{}

template<class K>
inline
poly<K>::poly(const z &e)
: impl(1,M(e))
{}

template<class K>
poly<K> poly<K>::zero;

template<class K>
poly<K> poly<K>::one(
  algebra::one<K>(),
  algebra::one<poly<K>::Z>()
);

template<class K>
inline bool
poly<K>::null() const {
  return impl::empty();
}

template<class K>
inline bool
poly<K>::unit() const {
  return impl::size() == 1 && impl::front().unit();
}

template<class K>
inline const typename poly<K>::z &
poly<K>::deg() const {
  return impl::front().deg();
}

template<class K>
inline poly<K> &
poly<K>::operator+=(const poly &o) {
  typename std::list<M>::iterator
    i1 = impl::begin(),
    e1 = impl::end();
  typename std::list<M>::const_iterator
    i2 = o.impl::begin(),
    e2 = o.impl::end();

  while(i1 != e1 && i2 != e2) {
    int cmp = algebra::compare(*i1, *i2);
    if(cmp < 0) { ++i1; continue; }
    if(cmp > 0) { impl::insert(i1, *i2); ++i2; continue; }
    *i1 += *i2;

    ++i2;

    if(algebra::null(*i1))
      i1 = impl::erase(*i1);
    else
      ++i1;
  }

  return *this;
}

template<class K>
inline poly<K> &
poly<K>::operator-=(const poly &o) {
  impl::iterator
    i1 = impl::begin(),
    e1 = impl::end();
  impl::const_iterator
    i2 = o.impl::begin(),
    e2 = o.impl::end();

  while(i1 != e1 && i2 != e2) {
    int cmp = algebra::compare(*i1, *i2);
    if(cmp < 0) { ++i1; continue; }
    if(cmp > 0) { impl::insert(i1, - *i2); ++i2; continue; }
    *i1 -= *i2;

    ++i2;

    if(algebra::null(*i1))
      i1 = impl::erase(*i1);
    else
      ++i1;
  }

  return *this;
}

template<class K>
inline poly<K> &
poly<K>::ineg() {
  foreach(mono &x, *this)
    x = -x;

  return *this;
}

template<class K>
inline poly<K> &
poly<K>::operator*=(const k &o) {
  if(algebra::null(o)) {
    impl::erase();
    return *this;
  }

  foreach(mono &x, *this)
    x *= o;

  return *this;
}
template<class K>
inline poly<K> &
poly<K>::operator/=(const k &o) {
  assert(! algebra::null(k));

  foreach(mono &x, *this)
    x /= o;

  return *this;
}

template<class K>
inline int
poly<K>::compare(const poly &a, const poly &b) {
  typename std::list<M>::const_iterator
    i1 = a.impl::begin(),
    e1 = a.impl::end(),
    i2 = b.impl::begin(),
    e2 = b.impl::end();

  for(; i1 != e1 && i2 != e2; ++i1, ++i2) {
    int cmp = M::deep_compare(*i1, *i2);
    if(cmp != 0) return cmp;
  }

  if(i1 != e1) return 1;
  if(i2 != e2) return-1;
  return 0;
}

template<class K>
inline poly<K>
operator+(poly<K> a, const poly<K> &b) {
  return a += b;
}

template<class K>
inline poly<K>
operator-(poly<K> a, const poly<K> &b) {
  return a -= b;
}

template<class K>
inline poly<K>
operator*(poly<K> a, const K &b) {
  return a *= b;
}

template<class K>
inline poly<K>
operator/(poly<K> a, const K &b) {
  return a /= b;
}

}} // poly::sparse

#include "multiply.hxx"

namespace poly {
namespace sparse {

namespace {

template<class K>
inline std::list<typename poly<K>::mono>
do_mul(const poly<K> &a, const poly<K> &b) {
  assert(!a.empty() && !b.empty());

  typedef typename poly<K>::mono mono;

  std::list<mono> ret;

  for(multiply::poly<poly<K> > muler(a,b);
      ! muler.empty();
      muler.next()) {
    const mono &m = muler.get();

    if(!algebra::null(m)) ret.push_back(m);
  }

  return ret;
}

}

template<class K>
inline poly<K> &
poly<K>::operator*=(const poly &o) {
  if(null() || o.null()) impl::clear();
  else impl::operator=(do_mul(*this, o));

  return *this;
}

template<class K>
inline poly<K>
operator*(poly<K> a, const poly<K> &b) {
  return a *= b;
}

}} // poly::sparse

#endif /* POLY_INL_ */
