#ifndef ANALYISIS_EXPR_WRAP_HPP
#define ANALYISIS_EXPR_WRAP_HPP

#include <caml/mlvalues.h>

#include "analysis/expr.hpp"
#include "util/move.hpp"

namespace analysis {

value expr_copy(const expr &);

#ifdef BOOST_HAS_RVALUE_REFS // c++0x
value expr_move(const expr&&);
#else
value expr_move(const expr &);
#endif

} // namespace analysis

#define Expr_val( x ) \
  ( reinterpret_cast< const ::analysis::expr* >( Data_custom_val( (x) ) ) )

#endif
