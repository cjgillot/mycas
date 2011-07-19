/*
 * expr.hpp
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef EXPR_HPP_
#define EXPR_HPP_

#include "stdlib.hpp"
#include "operators.hpp"

#include "analysis/basic.hpp"

#include "analysis/ptr.hpp"

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

  /*!
   * \brief Shared pointer representation
   * \invariant m_impl != 0
   */
  mutable boost::intrusive_ptr<const basic> m_impl;

public:
  //! \brief Copy constructor
  expr(const expr &o)
  : m_impl(o.m_impl) {}
  //! \brief Assignment operator
  expr& operator=(const expr &o) {
    m_impl = o.m_impl;
    return *this;
  }

  //! \brief Non-throwing swap
  void swap(expr &o) {
    m_impl.swap(o.m_impl);
  }

  //! \brief Non-virtual destructor
  ~expr()
  {}

  //! \brief Explicit creation from basic
  explicit
  expr(const basic* b)
  : m_impl(b) {}

public:
  //! \brief Nullity test
  bool null() const;
  //! \brief Unity test
  bool unit() const;

  //! \brief Printing
  inline void
  print(std::basic_ostream<char> &os) const
  { m_impl->print(os); }

  inline std::size_t
  get_hash() const
  { return m_impl->get_hash(); }

  static const unsigned default_eval_depth;
  void eval(unsigned = default_eval_depth) const;

public: // RTTI
  //! \brief \c basic pointer access
  inline const basic*
  get() const
  { return m_impl.get(); }

  template<class T>
  bool is_a() const
  { return dynamic_cast<const T*>( get() ); }

  template<class T>
  bool is_exactly_a() const
  { return typeid(get()) == typeid(const T*); }

  template<class T>
  const T* as_a() const
  { return static_cast<const T*>(get()); }

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
  static int
  compare(const expr &a, const expr &b);
};

inline std::basic_ostream<char> &
operator<<(std::basic_ostream<char> &os, const expr &e)
{ e.print(os); return os; }

} // namespace analysis

#endif /* EXPR_HPP_ */
