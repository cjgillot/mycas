/*
 * poly.hxx
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

#ifndef POLY_HXX_
#define POLY_HXX_

#include "monomial.hxx"

namespace poly {
namespace sparse {

template<class K>
struct poly {
  typedef monomial<K> M;

  typedef typename M::Z Z;

  typedef typename M::k k;
  typedef typename M::z z;

  typedef M mono;

  typedef z e;

  typedef std::list<monomial<K> > list_t;

  typedef typename list_t::iterator iterator;
  typedef typename list_t::const_iterator const_iterator;

  list_t impl;

public:
  poly();
  poly(const poly &m);
  poly &operator=(const poly &);

  poly(const k &, const z &);
  explicit
  poly(const z &);

  ~poly();

public:
  /* range concept */
  iterator begin() { return impl.begin(); }
  iterator end()   { return impl.end(); }

  const_iterator begin() const { return impl.begin(); }
  const_iterator end()   const { return impl.end(); }

  bool empty()  const { return impl.empty(); }
  size_t size() const { return impl.size();  }

public:
  static poly zero;
  static poly one;

  bool null() const;
  bool unit() const;

  const z &deg() const;

  bool monome() const;

  poly &operator+=(const poly &o);
  poly &operator-=(const poly &o);

  poly &operator*=(const k &o);
  poly &operator/=(const k &o);

  poly &operator*=(const poly &o);

  poly &ineg();
  poly  neg() const;

  static int compare(const poly &a, const poly &b);
};

// TODO: polynomial division

}} // poly::sparse

#include "poly.inl"

#endif /* POLY_HXX_ */
