#ifndef BASIC_IPP
#define BASIC_IPP

#include "analysis/forward.hpp"

#include "analysis/expr.hpp"
#include "analysis/basic.hpp"

namespace analysis {

// cdtor
inline basic::basic() {}
inline basic::basic(const basic&) {}
inline basic::~basic() {}

// tests
inline bool basic::null() const { return false; }
inline bool basic::unit() const { return false; }

// eval
inline expr basic::eval(unsigned) const { return expr( this ); }

// has
inline bool basic::has(const symbol &) const { return false; }

// RTTI
inline bool basic::is_numeric() const { return false; }

}

#endif
