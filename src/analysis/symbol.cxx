/*
 * symbol.cxx
 *
 *  Created on: 3 juil. 2011
 *      Author: k1000
 */

#include "symbol.hxx"

namespace analysis {

namespace {

inline const char*
get_name(const std::string &orig) {
  char* ret = new char[orig.length()+1];
  std::strcpy(ret, orig.c_str());
  return ret;
}

}

symbol::symbol() {}
symbol::~symbol() {}

symbol* symbol::clone() const {
  return const_cast<symbol*>(this);
}

void
symbol::print(std::basic_ostream<char> &os) const {
  os << '_' << static_cast<const void*>(this);
}

// all symbols are unique
// so address comparison is sufficient
int
symbol::compare_same_type(const basic &o) const {
  return this - static_cast<const symbol*>(&o);
}

ident::ident(const std::string &n)
: m_name(get_name(n)) {}
ident::~ident() {
  delete[] m_name;
}

void
ident::print(std::basic_ostream<char> &os) const {
  os << m_name;
}

constant::constant(const std::string &n, const number &v)
: ident(n), m_value(v) {}
constant::~constant() {}

}
