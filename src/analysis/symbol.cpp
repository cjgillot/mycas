/*
 * symbol.cpp
 *
 *  Created on: 3 juil. 2011
 *      Author: k1000
 */

#include "symbol.hpp"

namespace analysis {

namespace {

inline const char*
get_name(const std::string &orig) {
  char* ret = new char[orig.length()+1];
  std::strcpy(ret, orig.c_str());
  return ret;
}

}

symbol_::symbol_() {}
symbol_::~symbol_() {}

symbol_* symbol_::clone() const {
  return const_cast<symbol_*>(this);
}

void
symbol_::print(std::basic_ostream<char> &os) const {
  os << '_' << static_cast<const void*>(this);
}

// all symbols are unique
// so address comparison is sufficient
int symbol_::compare_same_type(const basic &o) const
{ return this - static_cast<const symbol_*>(&o); }

std::size_t symbol_::calc_hash() const
{ return boost::hash_value(this); }


bool symbol_::has(const symbol &s) const
{ return s.m_value.get() == this; }

expr symbol_::diff(const symbol &s, unsigned n) const
{
  if( n == 0 )
    return expr(this);

  if( n == 1 && has(s) )
    return number::one();

  return number::zero();
}


// ident class
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
