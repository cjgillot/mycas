/*
 * poly.hxx
 *
 *  Created on: 9 juin 2011
 *      Author: k1000
 */

#ifndef POLY_HXX_
#define POLY_HXX_

#include "operators.hxx"

#include "algebra/integer.hxx"
#include "algebra/compare.hxx"
#include "utils.hxx"

#include "polynomial/dense/multiply.hxx"

namespace poly {
namespace dense {

template<class K>
struct poly
: boost::arithmetic1<poly<K>
, boost::multiplicative2<poly<K>, K
, operators::ordered<poly<K>
, operators::printable<poly<K>
> > > > {
  typedef algebra::integer Z;

  typedef K k;
  typedef Z z;

  typedef std::vector<K> impl_t;

  typedef typename impl_t::iterator iterator;
  typedef typename impl_t::const_iterator const_iterator;

  // least exponent first -> ease shrinking
  impl_t impl;

private:
  // call after any modification
  // modifying the degree
  inline void
  reduce() {
    while(! impl.empty())
      if(algebra::null(impl.back()))
        impl.pop_back();
      else
        return;
  }

public:
  inline
  poly() {}
  inline
  poly(const poly &o)
  : impl(o.impl) {}
  inline poly &
  operator=(const poly &o) {
    impl = o.impl;
    return *this;
  }

public:
  inline explicit
  poly(const k &c)
  : impl(1,c)
  { if(algebra::null(c)) impl.clear(); }

  inline
  ~poly() {}

public:
  inline void
  swap(poly &o) {
    std::swap(impl, o.impl);
  }

private: /// range concept
  // mutable
  inline iterator
  begin()
  { return impl.begin(); }
  inline iterator
  end()
  { return impl.end(); }

public:
  // const
  inline const_iterator
  begin() const
  { return impl.begin(); }
  inline const_iterator
  end() const
  { return impl.end(); }

public:
  static poly zero;
  static poly one;

  inline bool
  null() const
  { return impl.empty(); }
  inline bool
  unit() const {
    return impl.size() == 1
        && algebra::unit(impl.front());
  }

  inline z
  deg() const
  { return impl.size() - 1; }

  inline bool
  monome() const {
    reduce();
    if(impl.empty()) return false;
    const_iterator it = impl.begin(), end = impl.end();
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
    a.impl.reserve(std::max(a.impl.size(), b.impl.size()));

    iterator
      i1 = a.begin(),
      e1 = a.end();
    const_iterator
      i2 = b.begin(),
      e2 = b.end();

    for(; i1 != e1 && i2 != e2; ++i1, ++i2)
      f2(*i1, *i2);

    /* if terms remain in o */
    for(; i2 != e2; ++i2)
      a.impl.push_back(f1(*i2));

    a.reduce();
  }

public:
  inline poly &
  operator+=(const poly &o) {
    combine(*this, o,
        functor::identity<K>(),
        functor::plus_eq<K>()
    );
    return *this;
  }
  inline poly &
  operator-=(const poly &o) {
    combine(*this, o,
        functor::negate<K>(),
        functor::minus_eq<K>()
    );
    return *this;
  }

  inline poly &
  ineg() {
    boost::for_each(*this, algebra::ineg<K>);
    return *this;
  }
  inline poly &
  operator*=(const k &o) {
    if(algebra::null(o)) {
      impl.clear();
      return *this;
    }
    std::for_each(begin(), end(), functor::multiplies_by<K>(o));
    return *this;
  }
  inline poly &
  operator/=(const k &o) {
    assert(! algebra::null(o));
    std::for_each(begin(), end(), functor::divides_by<K>(o));
    return *this;
  }

protected:
  static inline void
  do_mul(const impl_t &a, const impl_t &b, impl_t &r) {
    if(a.empty() || b.empty()) {
      r.clear();
      return;
    }
    r.resize(a.size() + b.size() - 1, algebra::zero<K>());
    multiply::mul(a,b,r.begin());
  }

public:
  inline poly &
  operator*=(const poly &o) {
    impl_t ret;
    do_mul(impl, o.impl, ret);
    std::swap(impl, ret);
    reduce();
    return *this;
  }

public: /// construction
  template<class Range>
  static inline poly
  from_coefs(const Range &r) {
    poly ret;
    int sz = boost::size(r);
    ret.impl.reserve(sz);
    boost::copy(r, std::back_inserter(ret.impl));
    return ret;
  }

public: /// printing
  template<class S>
  inline void
  print(S &ios) const {
    algebra::print_range(impl, ios);
  }

public:
  inline static int
  compare(const poly &a, const poly &b) {
    return algebra::range_compare(
        a.impl, b.impl,
        algebra::compare<K>
    );
  }
};

}} // namespace poly::dense

#endif /* POLY_HXX_ */
