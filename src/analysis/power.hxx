/*
 * power.hxx
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#ifndef POWER_HXX_
#define POWER_HXX_

#include "basic.hxx"
#include "expr.hxx"
#include "numeric.hxx"

#include "expair.hxx"

namespace analysis {

class power;

struct power_traits {
  typedef power type;

  typedef expr coef_type;
  typedef expr rest_type;

  struct ep {
    // expair::null testing
    static bool null_pair(const coef_type &, const rest_type &);

    // expair::compare and expair::deep_compare
    static int  compare_coef(const coef_type &, const coef_type &);
    static int  compare_rest(const rest_type &, const rest_type &);

    // all operations :
    static void add_coef(coef_type &, const coef_type &);
    static void sub_coef(coef_type &, const coef_type &);
    static void mul_coef(coef_type &, const coef_type &);
    static void div_coef(coef_type &, const coef_type &);

    static void neg_coef(coef_type &);

    // expair::print
    static void print_pair(std::basic_ostream<char> &
    , const coef_type &, const rest_type &
    );
  };
};

/*!
 * \brief This is the main power representation class
 *
 * This structure represents the power {b^e}
 * using the expair { coef=e; rest=b }.
 */
class power
: public basic {

public:
  typedef power_traits traits;

  // actual expair member type
  typedef expair<traits> impl_t;

private:
  impl_t m_impl;

public:
  // coercion with expair
  power(const impl_t &i)
  : m_impl(i) {}
  operator impl_t() const
  { return m_impl; }

private: // disabled
  power();
  power& operator=(const power &);

private: // ctors
  power(const power &);
  power(const basic*, const basic*);

public:
  void swap(power &);
  virtual ~power();

public: // coercion
  power* clone() const;
  const power* as_power() const;

public: // tests
  bool null() const;
  bool unit() const;

  expr eval(unsigned) const;

public: // misc.
  int compare_same_type(const basic &) const;
  void print(std::basic_ostream<char> &) const;

public: // static
  static const power*
  from_be(const basic* b, const basic* e);

  static const power*
  from_1basic(const basic*);

  static const power*
  from_numeric(const numeric*);
};

}

#endif /* POWER_HXX_ */
