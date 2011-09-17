#ifndef BASIC_HPP_
#define BASIC_HPP_

#include "util/refcounted.hpp"

#include "analysis/forward.hpp"
#include "analysis/register.hpp"

#include <map>

namespace analysis {

/*!\brief Base expression class
 *
 * This class is the polymorphic base for all the
 * algebra representation classes.
 * It provides an embedded reference counter
 * for use with \c ptr via \c expr or \c ptr.
 */
class basic
: private util::nonassignable {

  REGISTER_BASE( basic )

protected:
  basic();
  basic(const basic &);

  //! \brief Virtual destructor
  virtual ~basic();

public:
  //! \brief Virtual clone
  virtual basic* clone() const = 0;

public:
  /*!\brief Nullity test
   * \return \c true if the expression is equivalent 0,
   *    \c false otherwise (if unknown)
   */
  virtual bool null() const;
  /*!\brief Unity test
   * \see basic::null()
   */
  virtual bool unit() const;

  /*!\brief Powering function
   *
   * Rationale : power of expressions often have very special
   * meaning and evaluation rules (for example <tt>abs(real)^2 -> real^2</tt>),
   * whose handling must be given to the basis class.
   *
   * \param expo the exponent
   * \return the evaluated expression of \c *this ^ \c expo
   */
  virtual expr pow(const expr &expo) const;

public: // has/match/subs
  /*!\brief Has function
   *
   * \param s : a symbol
   * \return \c true if \c s is contained in the expression
   */
  virtual bool has(const symbol &) const;

  typedef std::map< unsigned, expr > match_map;

  /*!\brief Match function
   *
   * \param p : a pattern
   * \param mm : a match map
   * \return \c true if \c p is matched in the expression
   *
   * Side effects :
   *   if <tt>match( p, mm )</tt> returns \c false,
   *   \c mm must not be modified.
   */
  bool match(const expr &, match_map &) const;

  /*!\brief Subs function
   *
   * \param mm : a match map
   * \return the expression with all
   *
   * Side effects :
   *   if <tt>match( p, mm )</tt> returns \c false,
   *   \c mm must not be modified.
   */
  virtual expr subs(const std::map<expr, expr> &) const;

protected:
  virtual bool match_same_type(const basic &, match_map &) const;

public:
  /*!\brief Evaluation function
   *
   * This function is meant to be overridden by all
   * the compound derived classes.
   * It evaluates the object, modifies it if needed
   * without changing the represented value, and returns the most
   * simple form.
   *
   * If you want to return \c this, you must return instead
   * <tt>this->basic::eval( lv )</tt> to avoid infinite loop.
   *
   * This function must not have any recursive call if \a lv == 0.
   *
   * All recursive calls must be made with parameter \a lv - 1.
   *
   * Default implementation : no-op
   *
   * \param lv : the recursion level
   * \return the evaluated form
   */
  virtual expr eval(unsigned lv) const;

  bool is_evaluated() const;

public:
  /*!\brief Differentiation function
   *
   * \param s : the symbol with respect to which differentiate
   * \param nth : the number of differentiations
   * \return the evaluated nth derivative of \c this
   */
  expr diff(const symbol &s, unsigned nth = 1) const;

private:
  /*!\brief Virtual differentiation function
   *
   * \param s : the symbol with respect to which differentiate
   * \return the evaluated derivative of \c this
   */
  virtual expr differentiate(const symbol &) const = 0;

public:
  /*!\brief Expansion function
   *
   * If you want to return \c this, you must return instead
   * <tt>this->basic::expand()</tt> to mark as expanded.
   *
   * \return the evaluated expanded version of \c this
   */
  virtual expr expand() const;

  bool is_expanded() const;

public:
  //! \brief RTTI
  virtual bool is_numeric() const;

  /*!\name Coercions
   *
   * These functions return a degenerated form of
   * themselves in the specified format (sum/prod/power...).
   *
   * If they already are, just return \c this.
   *
   * \{
   */
  //@{
  virtual const sum*   as_sum  () const;
  virtual const prod*  as_prod () const;
  virtual const power* as_power() const;
  //@}
  /*! \} */

public:
  //! \brief Virtual printing function
  virtual void print(std::basic_ostream<char>&) const = 0;

private:
  /*!\brief Virtual homogeneous comparison
   *
   * \invariant This method is always called with
   * a basic of the same derived type.
   *
   * \return the comparison integer
   */
  virtual util::cmp_t compare_same_type(const basic&) const = 0;

public:
  /*!\brief Virtual hashing function
   *
   * \return a hash value for *this
   */
  virtual std::size_t hash() const = 0;

  /*!\brief Comparison dispatch function
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
  static util::cmp_t compare(const basic&, const basic&);

private: // flags, mutable to allow update by basecase eval() and expand()
  struct {
    bool evaluated : 1;
    bool expanded  : 1;
  } mutable m_flags;
};

}

#endif /* BASIC_HPP_ */
