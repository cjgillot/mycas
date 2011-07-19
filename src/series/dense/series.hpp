/*
 * series.hpp
 *
 *  Created on: 5 juin 2011
 *      Author: k1000
 */

#ifndef DENSE_SERIES_HPP_
#define DENSE_SERIES_HPP_

#include "stream.hpp"

#include "polynomial/dense/poly.hpp"

#include "series/dense/sum.hpp"

namespace series {
namespace dense {

template<class> class series;

using namespace streams;

namespace multiply {
template<class K, class Mem>
inline stream_ptr<K, Mem>
do_mul(const series<K> &, const series<K> &);
}

template<class K>
class series {
  typedef stream_ptr<K, std::deque<K> > impl_t;

public:
  typedef algebra::integer z;
  typedef K k;

  typedef std::deque<K> list_t;

  typedef typename impl_t::iterator iterator;
  typedef typename impl_t::const_iterator const_iterator;

private:
  mutable impl_t impl;

  series(stream_base<K, list_t>* s)
  : impl(s) {}

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
    std::vector<K> array(e);
    array.back() = c;
    impl=impl_t::from_range(array);
  }
  explicit inline
  series(const k &c) {
    K array[1] = { c };
    impl=impl_t::from_range(array);
  }

  inline
  ~series() {}

private:
  inline explicit
  series(iterator_base<K> *it)
  : impl(it) {}

  inline explicit
  series(const impl_t &s)
  : impl(s) {}

  static inline series
  from_iter_base(iterator_base<K>* it) {
    return series(impl_t(it));
  }

public:
  static inline series
  from_poly(const ::poly::dense::poly<K> &p) {
    return series(impl_t::from_range(p));
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
  static const series zero;
  static const series one;

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
  begin()
  { return impl.begin(); }
  inline const_iterator
  begin() const
  { return impl.begin(); }

  const std::deque<K> &
  values() const
  { return impl.values(); }

public:
  friend inline series
  operator+(const series &a, const series &b) {
    return from_iter_base(detail::combine(
        a.begin(), b.begin(),
        functor::identity<K>(), functor::identity<K>(),
        functor::plus<K>()
    ));
  }
  friend inline series
  operator-(const series &a, const series &b) {
    return from_iter_base(detail::combine(
        a.begin(), b.begin(),
        functor::identity<K>(), functor::negate<K>(),
        functor::minus<K>()
    ));
  }

  inline series
  operator-() const {
    return from_iter_base(streams::map_iter(
        begin(),
        functor::negate<K>()
    ));
  }

  friend inline series
  operator*(const series &a, const K &o) {
    if(algebra::null(o))
      return zero;
    return from_iter_base(streams::map_iter(
        a.begin(),
        functor::multiplies_right<K>(o)
    ));
  }
  friend inline series
  operator/(const series &a, const K &o) {
    assert(! algebra::null(o));
    return from_iter_base(streams::map_iter(
        a.begin(),
        functor::divides_right<K>(o)
    ));
  }

  friend inline series
  operator*(const series &a, const series &b) {
    return series(multiply::do_mul<K, list_t>(a,b));
  }
};

template<class K>
const series<K> series<K>::zero;

template<class K>
const series<K> series<K>::one(
  algebra::one<K>(),
  algebra::one<series<K>::Z>()
);


}} // series::dense

#include "series/dense/multiply.hpp"

#endif /* DENSE_SERIES_HPP_ */
