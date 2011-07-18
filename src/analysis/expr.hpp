/*
 * expr.hpp
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef EXPR_HXX_
#define EXPR_HXX_

#include "stdlib.hpp"
#include "operators.hpp"

#include "analysis/basic.hpp"

#include "analysis/ptr.hpp"

namespace analysis {

class expr
: public boost::field_operators1<expr
, operators::printable<expr
, operators::ordered<expr
> > > {

  typedef ptr<basic> impl_t;
  impl_t m_impl;

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

  inline void
  reset(const basic* p)
  { return m_impl.reset(p); }

public:
  inline bool
  null() const
  { return m_impl.null(); }
  inline bool
  unit() const
  { return m_impl.unit(); }

  inline void
  print(std::basic_ostream<char> &os) const
  { m_impl.print(os); }

  inline const basic*
  get() const
  { return m_impl.get(); }

  inline std::size_t
  get_hash() const
  { return m_impl.get()->get_hash(); }

  static const unsigned default_eval_depth;
  void eval(unsigned = default_eval_depth) const;

public: // RTTI
  template<class T>
  bool is_a() const
  { return dynamic_cast<const T*>(get()); }

  template<class T>
  bool is_exactly_a() const
  { return typeid(*get()) == typeid(const T); }

  template<class T>
  const T* as_a() const
  { return static_cast<const T*>(get()); }

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
