/*
 * add.hxx
 *
 *  Created on: 20 juin 2011
 *      Author: k1000
 */

#ifndef ADD_HXX_
#define ADD_HXX_

#include "analysis/expairseq.hxx"
#include "analysis/mul.hxx"

namespace analysis {

class add;

//! \brief Traits structure for addition \c expairseq
struct add_traits {
  //! \brief Actual implemented type
  typedef add type;

  //! \brief Addition overall coefficient type
  typedef number coef_type;

  /*!
   * \brief Operations with addition
   *
   * In the traits class, the \c expair is considered
   * as a multiplication.
   */
  struct ep {

    //! \brief Add the coefs
    static void mul_coef(coef_type &, const coef_type &);
    //! \brief Subtract the coefs
    static void div_coef(coef_type &, const coef_type &);

    //! \brief Negate the coef
    static void inv_coef(coef_type &);

    //! \brief Print the tree root '+'
    static void print_base(std::basic_ostream<char> &);

  };
};

/*!
 * \brief Addition class
 *
 * This \c expairseq-derived class represents
 * sums as a linear combination of multiplications,
 * whose coefficients are pure \c number's.
 *
 * Printing : (+ c m1 m2 m3 ...)
 */
class add
: public expairseq<add_traits, mul::traits> {

  //! \brief Base class type
  typedef expairseq<add_traits, mul::traits> super;

private:
  // disabled
  add();
  add &operator=(const add&);

protected:
  //! \brief Copy constructor (for clone)
  add(const add &);

public:
  //! \brief Destructor
  virtual ~add();

protected:
  //! \brief Number constructor
  explicit add(const number &);

  //! \brief Multiplication constructor
  add(const number &, const epair &);

public:
  add* clone() const
  { return new add(*this); }

public:
  virtual const add* as_add() const
  { return this; }

public:
  virtual bool null() const;
  virtual const basic* eval(unsigned) const;

public:
  add* iadd(const add&);
  add* isub(const add&);
  add* ineg();

  add* imul(const number&);

public:
  static add*
  from_1basic(const basic*);

  static add*
  from_numeric(const numeric*);
};

}

#endif /* ADD_HXX_ */
