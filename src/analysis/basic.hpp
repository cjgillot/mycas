/*
 * basic.hpp
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef BASIC_HPP_
#define BASIC_HPP_

#include "stdlib.hpp"
#include "util/refcounted.hpp"
#include "util/visitor.hpp"

#include "analysis/forward.hpp"

namespace analysis {

class expr;
class symbol;

//! \brief Flags used by basic
enum {
  Hashed = 1
};

/*!
 * \brief Base expression class
 *
 * This class is the polymorphic base for all the
 * algebra representation classes.
 * It provides an embedded reference counter
 * for use with \c boost::intrusive_ptr via \c expr or \c ptr.
 */
class basic
: public util::base_const_visitable<bool>
, private boost::noncopyable {

  MAKE_REFCOUNTED(basic);

  // shall be defined in every derived class
  DEFINE_CONST_VISITABLE()

public:
  //! \brief Default constructor
  basic();
  //! \brief Copy constructor
  basic(const basic &);
  //! \brief Virtual destructor
  virtual ~basic();

  //! \brief Non-throwing swap
  void swap(basic &);

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
   * \param lv : the recursion level
   * \return the evaluated form
   */
  virtual expr eval(unsigned lv) const;

  /*!
   * \brief Has function
   *
   * \param s : a symbol
   * \return \c true if \c s is contained in the expression
   */
  virtual bool has(const symbol &s) const = 0;

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
  virtual int compare_same_type(const basic&) const = 0;

  /*!
   * \brief Virtual hashing function
   *
   * Default implementation in basic :
   * return a hash of the typeid.
   *
   * At overriding, the super class hash should
   * be used as seed.
   *
   * \return a hash value for *this
   */
  virtual std::size_t
  calc_hash() const = 0;

public:
  //! \brief Hash value access
  std::size_t
  get_hash() const {
    if(m_flags & Hashed) return m_hash;
    m_hash = calc_hash();
    m_flags |= Hashed;
    return m_hash;
  }

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
  static int
  compare(const basic&, const basic&);

protected:
  /*!
   * \brief Forget function for the hash value
   *
   * This function has to be called each time
   * a basic is modified after its construction.
   */
  void unhash() {
    m_flags &= ~Hashed;
  }

private: // member data

  //! \brief Flags
  mutable unsigned m_flags;

  //! \brief Computed hash value
  mutable std::size_t m_hash;
};

}

#endif /* BASIC_HPP_ */
