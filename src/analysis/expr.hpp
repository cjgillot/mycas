/*
 * expr.hpp
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef EXPR_HPP_
#define EXPR_HPP_

#include "operators.hpp"

#include "util/compare.hpp"
#include "util/attribute.hpp"

#include "analysis/forward.hpp"

#include <boost/intrusive_ptr.hpp>

namespace analysis {

/*!
 * \brief Expression class
 *
 * \c expr is an intrusive pointer class,
 * forwarding \c basic operations.
 */
class expr
: public operators::printable<expr
, operators::ordered<expr
> > {

public:
  //! \brief Zero constructor
  expr();

  // compiler-generated copy and assignment

  //! \brief Non-throwing swap
  void swap(expr &o);
  //! \brief Non-virtual destructor
  ~expr();

  //! \brief Explicit creation from \c basic instance
  //! \param b a non-null pointer to a \c basic instance
  explicit expr(const basic*);

public:
  //! \brief Nullity test
  bool null() const;
  //! \brief Unity test
  bool unit() const;

  //! \brief Printing
  void print(std::basic_ostream<char> &os) const;

  static const unsigned default_eval_depth;
  void eval(unsigned = default_eval_depth) const;

  bool has(const symbol&) const;
  expr diff(const symbol&, unsigned=1) const;

public: // RTTI
  //! \brief \c basic pointer access
  const basic* get() const;

  bool is_numeric() const;
  template<class T> bool     is_a() const;
  template<class T> const T* as_a() const;
  template<class T> bool     is_exactly_a() const;

public:
  friend expr operator+(const expr &a)
  { return a; }
  friend expr operator-(const expr &a)
  { return a.neg(); }

  expr operator+(const expr&) const;
  expr operator-(const expr&) const;

  expr neg() const;

  expr operator*(const expr&) const;
  expr operator/(const expr&) const;

  expr inv() const;

  expr pow(const expr &) const;

public:
  expr &ineg() {
    neg().swap(*this);
    return *this;
  }
  expr &iinv() {
    inv().swap(*this);
    return *this;
  }

#define OP_EQ(op) \
  expr &operator op##=(const expr &o) { \
    ( operator op(o) ).swap(*this); \
    return *this; \
  }

  OP_EQ(+)
  OP_EQ(-)
  OP_EQ(*)
  OP_EQ(/)

#undef OP_EQ

public:
  static util::cmp_t
  compare(const expr &a, const expr &b);

  std::size_t hash() const;

private:
  /*!
   * \brief Shared pointer representation
   * \invariant m_impl != 0
   */
  mutable boost::intrusive_ptr<const basic> m_impl;
};

std::basic_ostream<char> &operator<<(std::basic_ostream<char> &, const expr &);

expr pow(const expr &, const expr &);

} // namespace analysis

#endif /* EXPR_HPP_ */
