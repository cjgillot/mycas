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

  // shall be defined in every derived class
  DEFINE_CONST_VISITABLE()

public:
  //! \brief Default constructor
  symbol_();
  //! \brief Destructor
  ~symbol_();

public:
  symbol_* clone() const;

public:
  void print(std::basic_ostream<char> &) const;
  expr diff(const symbol&, unsigned) const;
  bool has(const symbol&) const;

private:
  std::size_t calc_hash() const;
  int compare_same_type(const basic&) const;

  friend class symbol;
};

//! \brief Named symbol class
class ident
: public symbol_ {

  // shall be defined in every derived class
  DEFINE_CONST_VISITABLE()

public:
  //! \brief Constructor from string
  ident(const std::string &);
  //! \brief Destructor
  ~ident();

public:
  void print(std::basic_ostream<char> &) const;

private:
  const char* m_name;
};

//! \brief Constant symbol class
class constant
: public ident {

  // shall be defined in every derived class
  DEFINE_CONST_VISITABLE()

public:
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
  { return m_value->get_hash(); }

  static int compare(const symbol &a, const symbol &b)
  { return a.m_value->symbol_::compare_same_type(*b.m_value); }

private:
  friend class symbol_;

  boost::intrusive_ptr<const symbol_> m_value;
};

}

#endif /* SYMBOL_HPP_ */
