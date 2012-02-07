#include "analysis/vectorseq/expand/fwd.hpp"

namespace analysis {
namespace expand_detail {

namespace {

struct prod_expander
: std::unary_function<const prod*, ptr<const basic> >
{
  inline ptr<const basic> operator()( const prod* ep ) const
  { return prod_expand( *ep ); }
};

}

expr sum_expand(const sum &self)
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

}} // namespace analysis::expand_detail
