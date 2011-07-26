/*
 * power.hpp
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#ifndef POWER_HPP_
#define POWER_HPP_

#include "basic.hpp"
#include "expr.hpp"
#include "numeric.hpp"

#include "util/final.hpp"

namespace analysis {

class power;

DECLARE_FINAL_CLASS(power)

/*!
 * \brief This is the main power representation class
 *
 * This structure represents the power {b^e}
 * using the expair { coef=e; rest=b }.
 */
class power: FINAL_CLASS(power)
, public basic {

  DEFINE_CONST_VISITABLE()

public:
  struct handle;

private: // ctors
  power(const power &);
  power(const expr&, const expr&);

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
  expr diff(const symbol&, unsigned=1) const;
  bool has(const symbol&) const;

public: // misc.
  int compare_same_type(const basic &) const;
  void print(std::basic_ostream<char> &) const;

private:
  std::size_t calc_hash() const;

  /*!
   * \brief Logarithmic differentiation
   *
   * This method is used internally by \c prod::diff()
   * and \c power::diff().
   *
   * It returns the logarithmic derivative with respect to \c s.
   */
  expr diff_log(const symbol &s) const;

public: // static
  static const power*
  from_be(const basic* b, const basic* e);

  static const power*
  from_1basic(const basic*);

  static const power*
  from_numeric(const numeric*);

private:
  expr m_base, m_expo;
};

struct power::handle {

  handle(const power* p)
  : m_ptr(p) { assert(p); }
  ~handle() {}

  operator expr() const
  { return expr(m_ptr.get()); }


  handle operator+(const handle &) const;
  handle operator-(const handle &) const;
  handle operator-() const;


  bool null() const
  { return m_ptr->m_expo.null(); }

  static int compare(const handle &a, const handle &b);
  static int deep_compare(const handle &a, const handle &b) {
    int c = a.hash() - b.hash();
    if(c) return c;
    return a.m_ptr->power::compare_same_type(*b.m_ptr);
  }

  std::size_t hash() const
  { return m_ptr->power::get_hash(); }

  template<class S>
  void print(S &os) const
  { m_ptr->power::print(os); }

  const power* ptr() const
  { return m_ptr.get(); }

  bool has(const symbol &s) const
  { return m_ptr->power::has(s); }

  expr diff_log(const symbol &s) const
  { return m_ptr->power::diff_log(s); }

private:
  boost::intrusive_ptr<const power> m_ptr;
};

}

#endif /* POWER_HPP_ */
