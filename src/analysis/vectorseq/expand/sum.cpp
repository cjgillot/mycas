#include "analysis/vectorseq/expand/fwd.hpp"

using namespace analysis;
using namespace expand_detail;

struct prod_expander
: std::unary_function<const basic*, expr>
{
  inline expr
  operator()( const basic* ep ) const
  { return prod_expand( static_cast<const prod&>( *ep ) ); }
};

expr expand_detail::sum_expand(const sum &self)
{
  typedef std::vector< expr > exvec_t;
  util::scoped_ptr< exvec_t > children ( self.map_children( prod_expander() ) );

  // nothing has changed
  if( ! children )
    return self.basic::expand();

  // the sum was not degenerate
  ASSERT( ! children->empty() );

  if( children->size() == 1 )
  {
    const expr &ret = self.coef() + ( *children )[0];
    return ret.get()->basic::expand();
  }

  // at least one has changed
  ptr< sum > retp =
    sum::from_expr_range(
      children->begin()
    , children->end()
    );
  retp->coef() += self.coef();

//   foreach( const prod* p, *retp )
//     p->basic::expand();

  return retp->basic::expand();
}
