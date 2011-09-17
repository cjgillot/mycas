#ifndef BASIC_IPP
#define BASIC_IPP

#include "analysis/forward.hpp"

#include "analysis/expr.hpp"
#include "analysis/basic.hpp"

namespace analysis {

// cdtor
inline basic::basic()
: m_flags( ) {}
inline basic::basic(const basic &o)
: m_flags( o.m_flags ) {}
inline basic::~basic() {}

// tests
inline bool basic::null() const { return false; }
inline bool basic::unit() const { return false; }

// eval
inline expr basic::eval(unsigned) const
{
  m_flags.evaluated = true;
  return this;
}
inline bool basic::is_evaluated() const
{ return m_flags.evaluated; }

// expand
inline expr basic::expand() const
{
  m_flags.expanded = true;
  return this;
}
inline bool basic::is_expanded() const
{ return m_flags.expanded; }

// has
inline bool basic::has(const symbol &) const { return false; }

// RTTI
inline bool basic::is_numeric() const { return false; }

}

#endif
