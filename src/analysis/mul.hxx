/*
 * mul.hxx
 *
 *  Created on: 20 juin 2011
 *      Author: k1000
 */

#ifndef MUL_HXX_
#define MUL_HXX_

#include "analysis/expairseq.hxx"
#include "analysis/power.hxx"

namespace analysis {

class mul;

struct mul_traits {
  typedef mul type;

  typedef number coef_type;

  struct ep {

    static bool null_coef(const coef_type &);

    static void add_coef(coef_type &, const coef_type &);
    static void sub_coef(coef_type &, const coef_type &);
    static void mul_coef(coef_type &, const coef_type &);
    static void div_coef(coef_type &, const coef_type &);

    static void neg_coef(coef_type &);
    static void inv_coef(coef_type &);

    static void print_base(std::basic_ostream<char> &);

  };
};

class mul
: public expairseq<mul_traits, power::traits> {

  typedef expairseq<mul_traits, power::traits> super;

public:
  explicit
  mul(const super::impl_t &i)
  : super(i) {}

public:
  mul();

  mul(const mul &);
  void swap(mul &);

  virtual ~mul();

private:
  explicit
  mul(const number &);
  mul(const number &, const epair &);

public:
  mul* clone() const
  { return new mul(*this); }

public:
  virtual const mul* as_mul() const
  { return this; }

public:
  virtual bool null() const;
  virtual bool unit() const;
  virtual const basic* eval(unsigned) const;

public:
  mul* imul(const mul&);
  mul* idiv(const mul&);
  mul* iinv();

public:
  static mul*
  from_1basic(const basic*);

  static mul*
  from_numeric(const numeric*);
};

}

#endif /* MUL_HXX_ */
