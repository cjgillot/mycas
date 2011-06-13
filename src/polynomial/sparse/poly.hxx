/*
 * poly.hxx
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

// TODO:
// - polynomial division

#ifndef POLY_HXX_
#define POLY_HXX_

#include "stdlib.hxx"
#include "utils.hxx"

#include "algebra/compare.hxx"

#include "polynomial/sparse/monomial.hxx"
#include "polynomial/sparse/multiply.hxx"

namespace poly {
namespace sparse {

template<class K>
struct poly
: boost::arithmetic1<poly<K>
, boost::arithmetic2<poly<K>, monomial<K>
, boost::multiplicative2<poly<K>, K
, operators::ordered<poly<K>
, operators::printable<poly<K>
> > > > >
{
  typedef monomial<K> M;

  typedef typename M::Z Z;

  typedef typename M::k k;
  typedef typename M::z z;

  typedef M mono;

  typedef z e;

  // monomials are sorted
  // biggest exponent first
  typedef std::list<M> impl_t;

  impl_t impl;

  typedef typename impl_t::iterator iterator;
  typedef typename impl_t::const_iterator const_iterator;
  typedef typename impl_t::reverse_iterator reverse_iterator;
  typedef typename impl_t::const_reverse_iterator const_reverse_iterator;

public:
  inline
  poly() {}
  inline
  poly(const poly &m)
  : impl(m.impl) {}
  inline poly &
  operator=(const poly &o) {
    impl=o.impl;
    return *this;
  }

  inline
  poly(const k &c, const z &e)
  : impl(1,M(c,e)) {}
  explicit inline
  poly(const k &c)
  : impl(1,M(c)) {}

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
  inline reverse_iterator
  rbegin()
  { return impl.rbegin(); }
  inline reverse_iterator
  rend()
  { return impl.rend(); }


public:
  // const
  inline const_iterator
  begin() const
  { return impl.begin(); }
  inline const_iterator
  end() const
  { return impl.end(); }
  // const
  inline const_reverse_iterator
  rbegin() const
  { return impl.rbegin(); }
  inline const_reverse_iterator
  rend() const
  { return impl.rend(); }

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

  inline const z &
  deg() const {
    return impl.front().deg();
  }

  inline bool
  monome() const
  { return impl.size() == 1; }

public: /// construction
  template<class Range>
  static inline poly
  from_coefs(const Range &r) {
    poly ret;
    z exp(boost::size(r)-1);
    foreach(k c, r) {
      ret.impl.push_back(M(c, exp));
      exp -= 1;
    }
    return ret;
  }

public: /// printing
  template<class S>
  inline void
  print(S &ios) const {
    ios << " [ ";
    foreach(M m, impl)
      ios << m << " ; ";
    ios << " ] ";
  }

private:
  /// implementation of += and -= operators
  ///   requirement : m != 0, f1(m) != 0
  template<class Fnc1, class Fnc2>
  static inline void
  combine(impl_t &a, const impl_t &b, const Fnc1 &f1, const Fnc2 &f2) {
    iterator
      i1 = a.begin(),
      e1 = a.end();
    const_iterator
      i2 = b.begin(),
      e2 = b.end();

    while(i1 != e1 && i2 != e2) {
      int cmp = algebra::compare(*i1, *i2);
      if(cmp < 0) { ++i1; continue; }
      if(cmp > 0) { a.insert(i1, f1(*i2)); ++i2; continue; }
      f2(*i1, *i2);

      ++i2;

      if(algebra::null(*i1))
        i1 = a.erase(i1);
      else
        ++i1;
    }

    /* if terms remain in o */
    while(i2 != e2) {
      a.push_back(f1(*i2));
      ++i2;
    }
  }

public:
  inline poly &
  operator+=(const poly &o) {
    combine(
        impl, o.impl,
        functor::identity<M>(),
        functor::plus_eq<M>()
    );
    return *this;
  }
  inline poly &
  operator-=(const poly &o) {
    combine(
        impl, o.impl,
        functor::negate<M>(),
        functor::minus_eq<M>()
    );
    return *this;
  }

  inline poly &
  ineg() {
    boost::for_each(impl, algebra::ineg<M>);
    return *this;
  }
  inline poly &
  operator*=(const M &o) {
    if(algebra::null(o)) {
      impl.clear();
      return *this;
    }
    boost::for_each(
        impl,
        functor::multiplies_by<M>(o)
    );
    return *this;
  }
  inline poly &
  operator/=(const M &o) {
    assert(! algebra::null(o));
    boost::for_each(
        impl,
        functor::divides_by<M>(o)
    );
    return *this;
  }
  inline poly &
  operator*=(const k &o) {
    if(algebra::null(o)) {
      impl.clear();
      return *this;
    }
    boost::for_each(
        impl,
        functor::multiplies_by<M,k>(o)
    );
    return *this;
  }
  inline poly &
  operator/=(const k &o) {
    assert(! algebra::null(o));
    boost::for_each(
        impl,
        functor::divides_by<M,k>(o)
    );
    return *this;
  }

private:
  // implementation helper for operator*=
  static inline void
  do_mul(const impl_t &a, const impl_t &b, impl_t &r) {
    assert(!a.empty() && !b.empty());

    for(multiply::poly<poly> muler(a,b);
        ! muler.empty();
        muler.next()) {
      const mono &m = muler.get();

      if(!algebra::null(m)) r.push_back(m);
    }
  }

public:
  inline poly &
  operator*=(const poly &o) {
    if(impl.empty() || o.impl.empty()) {
      impl.clear();
      return;
    }

    impl_t ret;
    do_mul(impl, o.impl, ret);
    std::swap(impl, ret);

    return *this;
  }

public: /// comparison -> lexicographical
  static int
  compare(const poly &a, const poly &b) {
    return algebra::range_compare(
        a.impl, b.impl,
        M::deep_compare
    );
  }
};

template<class K>
poly<K> poly<K>::zero;

template<class K>
poly<K> poly<K>::one(
  algebra::one<K>(),
  algebra::one<poly<K>::Z>()
);

}} // poly::sparse

#endif /* POLY_HXX_ */
