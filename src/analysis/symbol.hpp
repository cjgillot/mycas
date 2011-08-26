/*
 * symbol.hpp
 *
 *  Created on: 3 juil. 2011
 *      Author: k1000
 */

#ifndef SYMBOL_HPP_
#define SYMBOL_HPP_

#include "analysis/basic.hpp"
#include "analysis/number.hpp"

namespace analysis {

class symbol;

//! \brief Anonymous symbol class
class symbol_
: public basic {

  REGISTER_CLASS( symbol_, basic )

  friend class symbol;

protected:
  //! \brief Default constructor
  symbol_();
  //! \brief Destructor
  ~symbol_();

protected:
  symbol_* clone() const;

protected:
  void print(std::basic_ostream<char> &) const;
  bool has(const symbol&) const;
  expr diff(const symbol&, unsigned) const;

private:
  std::size_t hash() const;
  util::cmp_t compare_same_type(const basic&) const;
};

//! \brief Named symbol class
class ident
: public symbol_ {

  REGISTER_CLASS( ident, symbol_ )

  friend class symbol;

protected:
  //! \brief Constructor from string
  ident(const std::string &);
  //! \brief Destructor
  ~ident();

protected:
  void print(std::basic_ostream<char> &) const;

private:
  std::string m_name;
};

//! \brief Constant symbol class
class constant
: public ident {

  REGISTER_CLASS( constant, ident )

  friend class symbol;

protected:
  //! \brief Constructor from string
  constant(const std::string &, const number &);
  //! \brief Destructor
  ~constant();

private:
  number m_value;
};

//! \brief Symbol handler
struct symbol {

  symbol()
  : m_value(new symbol_) {}

  symbol(const std::string &s)
  : m_value(new ident(s)) {}

  operator expr() const
  { return expr( m_value.get() ); }

  void swap(symbol &o)
  { m_value.swap(o.m_value); }

  std::size_t hash() const
  { return m_value->hash(); }

  static util::cmp_t compare(const symbol &a, const symbol &b)
  { return a.m_value->symbol_::compare_same_type(*b.m_value); }

private:
  friend class symbol_;

  boost::intrusive_ptr<const symbol_> m_value;
};

}

#endif /* SYMBOL_HPP_ */
