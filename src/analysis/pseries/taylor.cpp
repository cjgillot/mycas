#include "analysis/expr.hpp"

#include "analysis/pseries/pseries.hpp"
#include "analysis/pseries/repr.hpp"

#include "analysis/caml/expr_wrap.hpp"

using namespace analysis;

pseries* pseries::taylor(const expr &ex, const symbol &s)
{
  value self = expr_copy( ex );
  value symb = expr_copy( s );

  value ser = pseries_export_taylor( self, symb );

  ptr< pseries::repr > rep = new pseries::repr( ser );
  return new pseries( s, rep.get() );
}

expr basic::series(const symbol &s) const
{
  return pseries::taylor( this, s );
}
