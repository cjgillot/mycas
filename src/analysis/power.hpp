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

#include "expair.hpp"

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

public: // misc.
  int compare_same_type(const basic &) const;
  void print(std::basic_ostream<char> &) const;

private:
  std::size_t calc_hash() const;

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
  { return m_ptr->unit(); }

  static int compare(const handle &a, const handle &b);
  static int deep_compare(const handle &a, const handle &b) {
    int c = a.hash() - b.hash();
    if(c) return c;
    return a.m_ptr->power::compare_same_type(*b.m_ptr);
  }

  std::size_t hash() const
  { return m_ptr->get_hash(); }

  template<class S>
  void print(S &os) const
  { m_ptr->print(os); }

private:
  boost::intrusive_ptr<const power> m_ptr;
};

}

#endif /* POWER_HPP_ */
