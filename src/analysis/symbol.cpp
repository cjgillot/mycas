/*
 * symbol.cpp
 *
 *  Created on: 3 juil. 2011
 *      Author: k1000
 */

#include "symbol.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

using namespace analysis;

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
util::cmp_t symbol_::compare_same_type(const basic &o) const
{ return util::compare(this, static_cast<const symbol_*>(&o)); }

std::size_t symbol_::hash() const
{ return boost::hash_value(this); }

bool symbol_::has(const symbol &s) const
{ return s.get() == this; }


// ident class
ident::ident(const std::string &n)
: m_name(n) {}
ident::~ident() {}

void
ident::print(std::basic_ostream<char> &os) const {
  os << m_name;
}

constant::constant(const std::string &n, const number &v)
: ident(n), m_value(v) {}
constant::~constant() {}
