#ifndef ANALYSIS_OPERATORS_HPP
#define ANALYSIS_OPERATORS_HPP

#include "analysis/expr.hpp"
#include "analysis/number.hpp"

namespace analysis {

#define BIN_OP_DECL( op, arg1, arg2 ) \
  inline expr operator op( arg1, arg2 )

#define BIN_OP_IMPL( name, arg1, arg2 ) \
  { return expr::name( arg1, arg2 ); }

#define BIN_OP( op, name )                          \
  BIN_OP_DECL( op, const expr  &a, const expr  &b ) \
  BIN_OP_IMPL( name, a, b )                         \
  BIN_OP_DECL( op, const number&a, const expr  &b ) \
  BIN_OP_IMPL( name, a, b )                         \
  BIN_OP_DECL( op, const expr  &a, const number&b ) \
  BIN_OP_IMPL( name, a, b )

BIN_OP( +, add )
BIN_OP( -, sub )
BIN_OP( *, mul )
BIN_OP( /, div )

#undef BIN_OP
#undef BIN_OP_IMPL
#undef BIN_OP_DECL

inline const expr &operator+(const expr &e) { return e; }
inline expr        operator-(const expr &e) { return e.neg(); }

}

#endif
