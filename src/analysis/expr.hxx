/*
 * expr.hxx
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef EXPR_HXX_
#define EXPR_HXX_

#include "stdlib.hxx"
#include "operators.hxx"

#include "analysis/basic.hxx"

#include "analysis/ptr.hxx"

namespace analysis {

// FIXME : ptr<...> should be a simple member
class expr
: private ptr<basic>
, public boost::field_operators1<expr
, operators::printable<expr
, operators::ordered<expr
> > > {

  typedef ptr<basic> super;

  //! \brief Disabled default construtor
  expr();

public:
  expr(const expr &);
  expr& operator=(const expr &);

  void swap(expr&);

  ~expr();

  explicit
  expr(const basic*);

private:
  // copy before write function
  template<class T>
  T* cow();

public:
  using super::null;
  using super::unit;
  using super::print;
  using super::get;

  static const unsigned default_eval_depth;
  void eval(unsigned = default_eval_depth) const;

  bool is_numeric() const
  { return super::get() && super::get()->is_numeric(); }

  template<class T>
  bool is_a() const
  { return dynamic_cast<const T*>(super::get()); }

  template<class T>
  bool is_exactly_a() const
  { return typeid(*super::get()) == typeid(const T); }

  template<class T>
  const T* as_a() const
  { return static_cast<const T*>(super::get()); }

public:
  friend expr operator+(const expr &a)
  { return a; }
  friend expr operator-(const expr &a)
  { return expr(a).ineg(); }

  expr &ineg();

  expr &operator+=(expr);
  expr &operator-=(expr);

  expr &operator*=(expr);
  expr &operator/=(expr);

  expr &iinv();

  expr   pow(const expr &) const;

public:
  static int
  compare(const expr &a, const expr &b);
};

}

#endif /* EXPR_HXX_ */
