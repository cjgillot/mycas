/*
 * basic.hxx
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef BASIC_HXX_
#define BASIC_HXX_

#include "utils/refcounted.hxx"

#include "analysis/forward.hxx"

namespace analysis {

class basic
: public refcounted {

  std::size_t hash_value;

public:
  inline
  basic() {}

  inline
  basic(const basic &) {}
  inline basic&
  operator=(const basic &) {}
  inline void
  swap(basic &) {}

  inline virtual
  ~basic() {}

public:
  virtual basic* clone() const = 0;

protected:
  virtual add* as_add() const;

public:
  virtual basic* add(const basic*) const;
  virtual basic*iadd(const basic*);

  virtual basic* sub(const basic*) const;
  virtual basic*isub(const basic*);

  virtual basic* neg() const;
  virtual basic*ineg();

  virtual basic* mul(const basic*) const;
  virtual basic*imul(const basic*);

  virtual basic* div(const basic*) const;
  virtual basic*idiv(const basic*);

protected:
  virtual int compare(const basic*) const;

public:
  static int
  compare(const basic &a, const basic &b) {
    int c = a.hash_value - b.hash_value;
    return c != 0 ? c : a.compare(b);
  }
};

}

#endif /* BASIC_HXX_ */
