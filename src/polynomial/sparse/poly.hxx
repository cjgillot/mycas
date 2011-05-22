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
struct poly: public std::list<monomial<K> > {
  typedef monomial<K> M;

  typedef typename M::Z Z;

  typedef typename M::k k;
  typedef typename M::z z;

  typedef M mono;

  typedef z e;

private:
  typedef std::list<monomial<K> > impl;

public:
  poly();
  poly(const poly &m);
  poly &operator=(const poly &);

  poly(const k &, const z &);
  explicit
  poly(const z &);

  ~poly();

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
  poly &operator/=(const poly &o);

  poly &ineg();
  poly  neg() const;

  static int compare(const poly &a, const poly &b);
};

}} // poly::sparse

#include "poly.inl"

#endif /* POLY_HXX_ */
