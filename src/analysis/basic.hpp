/*
 * basic.hpp
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef BASIC_HPP_
#define BASIC_HPP_

#include "util/refcounted.hpp"

#include "analysis/forward.hpp"
#include "analysis/register.hpp"

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
: private util::nonassignable {

  REGISTER_BASE( basic )

protected:
  basic();
  basic(const basic &);

public:
  //! \brief Virtual destructor
  virtual ~basic();

  //! \brief Virtual clone
  virtual basic* clone() const = 0;

public:
  /*!
   * \brief Nullity test
   * \return \c true if the expression is exactly 0,
   *    \c false otherwise (if unknown)
   */
  virtual bool null() const;
  /*!
   * \brief Unity test
   * \see basic::null()
   */
  virtual bool unit() const;

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
   * Default implementation : no-op
   *
   * \param lv : the recursion level
   * \return the evaluated form
   */
  virtual expr eval(unsigned) const;

  /*!
   * \brief Has function
   *
   * \param s : a symbol
   * \return \c true if \c s is contained in the expression
   */
  virtual bool has(const symbol &) const;

  /*!
   * \brief Differentiation function
   *
   * \param s : the symbol with respect to which we differentiate
   * \param nth : the number of differentiations
   * \return the evaluated nth derivative of \c *this
   */
  virtual expr diff(const symbol &s, unsigned nth = 1) const = 0;

public:
  //! \brief RTTI
  virtual bool is_numeric() const;

  /*!
   * \name Coercions
   *
   * These functions return a degenerated form of
   * themselves in the specified format (sum/prod/power...).
   *
   * If they already are, just return \c this.
   *
   * \{
   */
  //! \brief Addition coercion
  virtual const sum* as_sum() const;
  //! \brief Multiplication coercion
  virtual const prod* as_prod() const;
  //! \brief Power coercion
  virtual const power* as_power() const;
  /*! \} */

public:
  //! \brief Virtual printing function
  virtual void
  print(std::basic_ostream<char>&) const = 0;

private:
  /*!
   * \brief Virtual homogeneous comparison
   *
   * \invariant This method is always called with
   * a basic of the same derived type.
   *
   * \return the comparison integer
   */
  virtual util::cmp_t compare_same_type(const basic&) const = 0;

public:
  /*!
   * \brief Virtual hashing function
   *
   * \return a hash value for *this
   */
  virtual std::size_t hash() const = 0;

  /*!
   * \brief Comparison dispatch function
   *
   * The comparison template is the following :
   * - first compare hash values : if different, return according to these
   * - then compare
   *
   * This function compares the \c typeid of the
   * two \c basic objects, and orders according to
   * these types.
   * If equal, \c compare_same_type() is called.
   *
   * \see compare_same_type()
   */
  static util::cmp_t
  compare(const basic&, const basic&);
};

}

#endif /* BASIC_HPP_ */
