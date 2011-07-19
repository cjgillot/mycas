/*
 * series.hpp
 *
 *  Created on: 5 juin 2011
 *      Author: k1000
 */

#ifndef SPARSE_SERIES_HPP_
#define SPARSE_SERIES_HPP_

#include "stream.hpp"

#include "polynomial/sparse/monomial.hpp"

#include "series/sparse/sum.hpp"
#include "series/sparse/multiply.hpp"

#include "polynomial/sparse/poly.hpp"

namespace series {
namespace sparse {

using namespace poly::sparse;
using namespace streams;

template<class K>
struct series {
  typedef monomial<K> M;

  typedef typename M::Z Z;

  typedef typename M::k k;
  typedef typename M::z z;

  typedef M mono;

  typedef z e;

  typedef stream<monomial<K> > impl_t;

  typedef typename impl_t::iterator iterator;
  typedef typename impl_t::const_iterator const_iterator;

  mutable impl_t impl;

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

  inline
  series(const k &c, const z &e) {
    M array[1] = { M(c,e) };
    impl=impl_t::from_range(array);
  }
  explicit inline
  series(const k &c) {
    M array[1] = { M(c) };
    impl=impl_t::from_range(array);
  }

  inline
  ~series() {}

private:
  inline explicit
  series(iterator_base<M> *it)
  : impl(it) {}

  inline explicit
  series(const impl_t &s)
  : impl(s) {}

  static inline series
  from_iter_base(iterator_base<M>* it) {
    return series(impl_t(it));
  }

public:
  static inline series
  from_poly(const ::poly::sparse::poly<K> &p) {
    return series(impl_t::from_range(p | boost::adaptors::reversed));
  }

public:
  template<class S>
  inline void
  print_n(int n, S &ios) {
    typename impl_t::iterator it = impl.begin();
    ios << "series<" << typeid(K).name() << ">[ ";
    for(int i = 1; i <= n && it; ++i, ++it)
      ios << *it << " ; ";
    ios << " ] ";
  }

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

public:
  inline iterator
  begin() const
  { return impl.begin(); }

public:
  friend inline series
  operator+(const series &a, const series &b) {
    return from_iter_base(detail::combine(
        a.begin(), b.begin(),
        functor::identity<M>(), functor::identity<M>(),
        functor::plus<M>()
    ));
  }
  friend inline series
  operator-(const series &a, const series &b) {
    return from_iter_base(detail::combine(
        a.begin(), b.begin(),
        functor::identity<M>(), functor::negate<M>(),
        functor::minus<M>()
    ));
  }

  inline series
  operator-() const {
    return from_iter_base(streams::map_iter(
        begin(),
        functor::negate<M>()
    ));
  }

  friend inline series
  operator*(const series &a, const M &o) {
    if(algebra::null(o))
      return zero;
    return from_iter_base(streams::map_iter(
        a.begin(),
        functor::multiplies_right<M>(o)
    ));
  }
  friend inline series
  operator/(const series &a, const M &o) {
    assert(! algebra::null(o));
    return from_iter_base(streams::map_iter(
        a.begin(),
        functor::divides_right<M>(o)
    ));
  }
  friend inline series
  operator*(const series &a, const k &o) {
    if(algebra::null(o))
      return zero;
    return from_iter_base(streams::map_iter(
        a.begin(),
        functor::multiplies_right<M,k>(o)
    ));
  }
  friend inline series
  operator/(const series &a, const k &o) {
    assert(! algebra::null(o));
    return from_iter_base(streams::map_iter(
        a.begin(),
        functor::divides_right<M,k>(o)
    ));
  }

  friend inline series
  operator*(const series &a, const series &b) {
    return from_iter_base(detail::prod(
      a.impl.iter()
    , b.impl.iter()
    ));
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


#endif /* SPARSE_SERIES_HPP_ */
