/*
 * series.hxx
 *
 *  Created on: 5 juin 2011
 *      Author: k1000
 */

#ifndef DENSE_SERIES_HXX_
#define DENSE_SERIES_HXX_

#include "imperative/stream.hxx"
#include "polynomial/sparse/monomial.hxx"

#include "series/sparse/map.hxx"
#include "series/sparse/add.hxx"
#include "series/sparse/multiply.hxx"

namespace series {
namespace sparse {

using poly::sparse::monomial;
using namespace imperative::stream;

template<class K>
struct series {
  typedef algebra::integer Z;

  typedef stream<K> impl_t;

  typedef typename impl_t::iterator iterator;
  typedef typename impl_t::const_iterator const_iterator;

  mutable boost::intrusive_ptr<impl_t> impl;

public:
  inline
  series() {}
  inline
  series(const series &m)
  : impl(m.impl) {}
  inline series &
  operator=(const series &o) {
    impl=o.impl;
    return *this;
  }

  /* TODO simple constructors
  inline
  series(const k &c, const z &e)
  : impl(1,M(c,e)) {}
  explicit inline
  series(const k &c)
  : impl(1,M(c)) {}
  */

  inline
  ~series() {}

private:
  inline
  series(iterator_base<K> *it)
  : impl(new impl_t(it)) {}

public:
  static series zero;
  static series one;

  inline bool
  null() const {
    if(! impl) return true;
    if(impl->empty()) {
      impl.reset();
      return true;
    }
    return false;
  }

private:
  inline iterator_base<M>*
  iter() const
  { return impl ? impl->iter() : 0; }

public:
  inline iterator
  begin() const
  { return impl ? impl->begin() : iterator(); }

public:
  friend inline series
  operator+(const series &a, const series &b) {
    return detail::combine(
        a.iter(), b.iter(),
        functor::identity<M>(), functor::identity<M>(),
        functor::plus<M>()
    );
  }
  friend inline series
  operator-(const series &a, const series &b) {
    return detail::combine(
        a.iter(), b.iter(),
        functor::identity<M>(), functor::negate<M>(),
        functor::minus<M>()
    );
  }

  inline series
  operator-() const {
    return detail::map(this->iter(), functor::negate<M>());
  }

  friend inline series
  operator*(const series &a, const M &o) {
    if(algebra::null(o))
      return zero;
    return detail::map(
        a.iter(),
        functor::multiplies_right<M>(o)
    );
  }
  friend inline series
  operator/(const series &a, const M &o) {
    assert(! algebra::null(o));
    return detail::map(
        a.iter(),
        functor::divides_right<M>(o)
    );
  }
  friend inline series
  operator*(const series &a, const k &o) {
    if(algebra::null(o))
      return zero;
    return detail::map(
        a.iter(),
        functor::multiplies_right<M,k>(o)
    );
  }
  friend inline series
  operator/(const series &a, const k &o) {
    assert(! algebra::null(o));
    return detail::map(
        a.iter(),
        functor::divides_right<M,k>(o)
    );
  }

  friend inline series
  operator*(const series &a, const series &b) {
    return detail::mul(a.iter(), b.iter());
  }
};

template<class K>
series<K> series<K>::zero;

template<class K>
series<K> series<K>::one(
  algebra::one<K>(),
  algebra::one<series<K>::Z>()
);


}} // series::sparse


#endif /* DENSE_SERIES_HXX_ */
