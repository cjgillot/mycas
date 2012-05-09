#ifndef ANALYISIS_EXPR_WRAP_HPP
#define ANALYISIS_EXPR_WRAP_HPP

#include <caml.hpp>

#include "analysis/expr.hpp"

namespace analysis {

value expr_copy(const expr&);
value expr_move(expr&&);

} // namespace analysis

#define Expr_val( x ) \
  ( reinterpret_cast< const ::analysis::expr* >( Field((x),2) ) )

#endif
