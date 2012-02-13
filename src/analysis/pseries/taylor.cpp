#include "analysis/expr.hpp"

#include "analysis/pseries/pseries.hpp"
#include "analysis/pseries/repr.hpp"

#include "caml.hpp"

#include "analysis/pseries/export.h"

#include "analysis/caml/expr_wrap.hpp"

using namespace analysis;

expr basic::series(const symbol &s) const
{
  value self = expr_copy( this );
  value symb = expr_copy( s );

  value ser = pseries_export_taylor( self, symb );

  ptr< pseries::repr > rep = new pseries::repr( ser );
  return new pseries( s, rep.get() );
}
