#ifndef ANALYSIS_OPERATORS_HPP
#define ANALYSIS_OPERATORS_HPP

#include "analysis/basic/expr.hpp"

namespace analysis {

//! \name Operator overloads
//! \{
#define BIN_OP( op, name )  \
  inline expr& expr::operator op##=(const expr &o)        \
  { expr::name( *this, o ).swap( *this ); return *this; } \
  inline expr operator op(const expr &a, const expr &b)   \
  { return expr::name( a, b ); }

BIN_OP( +, add )
BIN_OP( -, sub )
BIN_OP( *, mul )
BIN_OP( /, div )

#undef BIN_OP

inline expr&
expr::ineg()
{
  expr::neg( *this ).swap( *this );
  return *this;
}
inline expr&
expr::iinv()
{
  expr::inv( *this ).swap( *this );
  return *this;
}

inline const expr &operator+(const expr &e) { return e; }
inline expr        operator-(const expr &e) { return expr::neg(e); }

inline expr
pow(const expr &b, const expr &e)
{ return expr::pow(b, e); }

//! \}

}

#endif
