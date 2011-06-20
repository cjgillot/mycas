/*
 * add.hxx
 *
 *  Created on: 20 juin 2011
 *      Author: k1000
 */

#ifndef ADD_HXX_
#define ADD_HXX_

#include "analysis/basic.hxx"

#include "polynomial/sparse/monomial.hxx"
#include "polynomial/sparse/poly.hxx"

namespace analysis {

class add
: public basic {

  typedef poly::sparse::monomial<number*, mul*> mono_t;
  typedef poly::sparse::poly<mono_t> poly_t;

  poly_t poly;

public:
  inline
  add() {}

  inline
  add(const add &) {}
  inline add&
  operator=(const add &) {}
  inline void
  swap(add &) {}

  inline virtual
  ~add() {}

public:
  virtual add* clone() const
  { return new add(poly_t(impl)); }

protected:
  virtual add* as_add() const
  { return this; }

public:
  virtual add*  add(const basic*) const;
  virtual add* iadd(const basic*);

  virtual add*  sub(const basic*) const;
  virtual add* isub(const basic*);

  virtual add*  neg() const;
  virtual add* ineg();
};

}

#endif /* ADD_HXX_ */
