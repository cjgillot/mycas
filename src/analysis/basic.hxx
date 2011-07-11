/*
 * basic.hxx
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef BASIC_HXX_
#define BASIC_HXX_

#include "stdlib.hxx"
#include "util/refcounted.hxx"
#include "util/visitor.hxx"

#include "analysis/forward.hxx"

namespace analysis {

/*!
 * \brief Base expression class
 *
 * This class is the polymorphic base for all the
 * algebra representation classes.
 * It provides an embedded reference counter
 * for use with \c boost::intrusive_ptr via \c expr or \c ptr.
 */
class basic
: public util::refcounted
, public util::base_const_visitable<bool> {

  // shall be defined in every derived class
  DEFINE_CONST_VISITABLE()

public:
  //! \brief Default constructor
  basic();
  //! \brief Copy constructor
  basic(const basic &);
  //! \brief Virtual destructor
  virtual ~basic();

  //! \brief Virtual clone
  virtual basic* clone() const = 0;

public:
  /*!
   * \brief Evaluation function
   *
   * This function is meant to be overridden by all
   * the compound derived classes.
   * It evaluates the object, modifies it if needed
   * (without changing the value), and returns the most
   * simple form (often \c this, or a number).
   *
   * This function must not recurse if \a lv == 0.
   *
   * All recursive calls must ba made with parameter \a lv - 1.
   *
   * @param lv : the recursion level
   * @return the evaluated form
   */
  virtual expr eval(unsigned lv) const;

  /*!
   * \brief Nullity test
   * \return true if the expression is exactly 0,
   *    false otherwise (if unknown)
   */
  virtual bool null() const;
  /*!
   * \brief Unity test
   * \see basic::null()
   */
  virtual bool unit() const;

public:
  /*!
   * \brief Copy on write function
   *
   * Calling this function allows to get
   * a modifiable version of the current object.
   *
   * \return \c this if modifiable, a \c clone() otherwise.
   */
//  template<class T>
//  T* cow() const {
//    if(!util::unique(this))
//      return static_cast<T*>(clone());
//    return static_cast<T*>(const_cast<basic*>(this));
//  }

public:
  //! \brief RTTI
  virtual bool is_numeric() const;

  /*!
   * \defgroup Coercions
   *
   * These functions return a degenerated form of
   * themselves in the specified format (add/mul/power...).
   *
   * If they already are, just return \c this.
   *
   * \{
   */
  //! \brief Addition coercion
  virtual const add* as_add() const;
  //! \brief Multiplication coercion
  virtual const mul* as_mul() const;
  //! \brief Power coercion
  virtual const power* as_power() const;
  /*! \} */

public:
  //! \brief Virtual printing function
  virtual void
  print(std::basic_ostream<char>&) const = 0;

public:
  /*!
   * \brief Virtual homogeneous comparison
   *
   * \invariant This method is always called with
   * a basic of the same derived type.
   *
   * \return the comparison integer
   */
  virtual int compare_same_type(const basic&) const = 0;

  /*!
   * \brief Comparison dispatch function
   *
   * This function compares the \c typeid of the
   * two \c basic objects, and orders according to
   * these types.
   * If equal, compare_same_type is called.
   */
  static int
  compare(const basic&, const basic&);
};

}

#endif /* BASIC_HXX_ */
