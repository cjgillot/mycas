#include "analysis/expand/fwd.hpp"

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

// prod
expr prod_expand(const prod &self)
{
  { // trivial case ?
    foreach( const power* p, self )
      if( ( ! p->expo().is_numeric() )
       || ( ! p->base().is_a< symbol_ >() ) )
        goto nontrivial;

    self.basic::expand();
    return &self;

  nontrivial:
    ;
  }

  typedef std::vector< expr > exvec_t;
  util::scoped_ptr< exvec_t > children ( self.map_children( power_expander() ) );

  // nothing has changed
  if( ! children )
  {
    // avoid the case : expanded * expanded sum
    foreach( const power* p, self )
      if( ( p->expo().is_numeric() )
       && ( p->base().is_a< sum >() ) )
        goto has_sum;

    return self.basic::expand();

  has_sum:
    children.reset( new exvec_t( self.begin(), self.end() ) );
  }

  ASSERT( ! children->empty() );

  if( children->size() == 1 )
  {
    const expr &ret = self.coef() * children->front();

    // valid since prod::eval distributes the constant
    // over the sum
    return ret.get()->basic::expand();
  }

  // we have to expand a variadic product of variadic sums

  // store those which aren't sums
  //   The bound sz = children->size() is enough
  //   since the loop runs exactly sz times
  //   and pushes at most once per run.
  //   + 1 for the coefficient
  // this sequence represents a _product_
  container::unsafe_vector< expr > non_sum_seq ( 1 + children->size() );
  non_sum_seq.push_back( self.coef() );

  // store the current expansion state
  expr last_sum ( number::one() );

  foreach( const expr &ex, *children )
    if( ! ex.is_a< sum >() )
      non_sum_seq.push_back( ex );

    else if( ! last_sum.is_a< sum >() )
    { // got a sum
      if( ! last_sum.unit() )
        non_sum_seq.push_back( last_sum );

      last_sum = ex;
    }

    else
    { // got a product of sums (last_sum * ex)
      const sum &last = *last_sum.as_a< sum >();
      const sum &next = *ex.as_a< sum >();
      expr( expand_sum_sum( last, next ) ).swap( last_sum );
    }

  // now, we have to build and return the product last_sum * non_sum

  // trivial case : last_sum gets in non_sum_seq
  if( ! last_sum.is_a< sum >() )
  {
    if( ! last_sum.unit() )
      // if we reach here, at least one sum * sum case has happened,
      // so non_sum_seq has still some room
      non_sum_seq.push_back( last_sum );

    ptr< prod > ret =
      prod::from_expr_range(
        non_sum_seq.begin()
      , non_sum_seq.end()
      );
    ret->basic::expand();
    return ret;
  }

  const expr &non_sum = prod::from_expr_range( non_sum_seq.begin(), non_sum_seq.end() );
  non_sum_seq.clear();

  const sum  &last = *last_sum.as_a< sum >();

  return expand_prod_sum( non_sum, last );
}


}} // namespace analysis::expand_detail
