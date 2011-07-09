/*
 * symbol.hxx
 *
 *  Created on: 3 juil. 2011
 *      Author: k1000
 */

#ifndef SYMBOL_HXX_
#define SYMBOL_HXX_

#include "stdlib.hxx"
#include "basic.hxx"

namespace analysis {

/*!
 * \class symbol
 *
 * \brief Anonymous symbol class
 */
class symbol
: public basic {

public:
  symbol();
  ~symbol();

public:
  symbol* clone() const;

public:
  void print(std::basic_ostream<char> &) const;

  int compare_same_type(const basic&) const;
};

/*!
 * \class ident
 *
 * \brief Named symbol class
 */
class ident
: public symbol {

  const char* m_name;

private:
  ident();

public:
  //! \brief Constructor from string
  ident(const std::string &);
  ~ident();

public:
  void print(std::basic_ostream<char> &) const;
};

}

#endif /* SYMBOL_HXX_ */