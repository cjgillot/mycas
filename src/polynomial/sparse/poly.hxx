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
#include "monomial.hxx"
#include "multiply.hxx"

namespace poly {
namespace sparse {

template<class K>
struct poly
: public std::list<monomial<K> >
, boost::arithmetic1<poly<K>
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
  typedef std::list<M> impl;

  typedef typename impl::iterator iterator;
  typedef typename impl::const_iterator const_iterator;

public:
  inline
  poly() {}
  inline
  poly(const poly &m)
  : impl(m) {}
  inline poly &
  operator=(const poly &o) {
    impl::operator=(o);
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
  static poly zero;
  static poly one;

  inline bool
  null() const
  { return impl::empty(); }
  inline bool
  unit() const {
    return impl::size() == 1
        && algebra::unit(impl::front());
  }

  inline const z &
  deg() const {
    return impl::front().deg();
  }

  inline bool
  monome() const
  { return impl::size() == 1; }

public: /// construction
  template<class Range>
  static inline poly
  from_coefs(const Range &r) {
    poly ret;
    z exp(boost::size(r)-1);
    foreach(k c, r) {
      ret.impl::push_back(M(c, exp));
      exp -= 1;
    }
    return ret;
  }

public: /// printing
  template<class S>
  inline void
  print(S &ios) const {
    ios << " [ ";
    foreach(M m, *this)
      ios << m << " ; ";
    ios << " ] ";
  }

private:
  /// implementation of += and -= operators
  ///   requirement : m != 0, f1(m) != 0
  template<class Fnc1, class Fnc2>
  static inline void
  combine(poly &a, const poly &b, const Fnc1 &f1, const Fnc2 &f2) {
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
        *this, o,
        functor::identity<M>(),
        functor::plus_eq<M>()
    );
    return *this;
  }
  inline poly &
  operator-=(const poly &o) {
    combine(
        *this, o,
        functor::negate<M>(),
        functor::minus_eq<M>()
    );
    return *this;
  }

  inline poly &
  ineg() {
    boost::for_each(*this, algebra::ineg<M>);
    return *this;
  }
  inline poly &
  operator*=(const M &o) {
    if(algebra::null(o)) {
      impl::clear();
      return *this;
    }
    boost::for_each(
        *this,
        functor::multiplies_by<M>(o)
    );
    return *this;
  }
  inline poly &
  operator/=(const M &o) {
    assert(! algebra::null(o));
    boost::for_each(
        *this,
        functor::divides_by<M>(o)
    );
    return *this;
  }
  inline poly &
  operator*=(const k &o) {
    if(algebra::null(o)) {
      impl::clear();
      return *this;
    }
    boost::for_each(
        *this,
        functor::multiplies_by<M,k>(o)
    );
    return *this;
  }
  inline poly &
  operator/=(const k &o) {
    assert(! algebra::null(o));
    boost::for_each(
        *this,
        functor::divides_by<M,k>(o)
    );
    return *this;
  }

private:
  // implementation helper for operator*=
  static inline impl
  do_mul(const poly &a, const poly &b) {
    assert(!a.empty() && !b.empty());

    typedef typename poly<K>::mono mono;

    std::list<mono> ret;

    for(multiply::poly<poly> muler(a,b);
        ! muler.empty();
        muler.next()) {
      const mono &m = muler.get();

      if(!algebra::null(m)) ret.push_back(m);
    }

    return ret;
  }

public:
  inline poly &
  operator*=(const poly &o) {
    if(impl::empty() || o.empty()) impl::clear();
    else impl::operator=(do_mul(*this, o));

    return *this;
  }

public: /// comparison -> lexicographical
  static int
  compare(const poly &a, const poly &b) {
    typename std::list<M>::const_iterator
      i1 = a.begin(),
      e1 = a.end(),
      i2 = b.begin(),
      e2 = b.end();

    for(; i1 != e1 && i2 != e2; ++i1, ++i2) {
      int cmp = M::deep_compare(*i1, *i2);
      if(cmp != 0) return cmp;
    }

    // one at least is empty
    if(i1 != e1) return 1;
    if(i2 != e2) return-1;
    // both empty
    return 0;
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
