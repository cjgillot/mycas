#include "analysis/expand/fwd.hpp"

namespace analysis {
namespace expand_detail {

/*!\brief Expand an expression of the form scale * addition
 *
 * Provided \c scale is not a sum, expand the expression
 * <tt>scale * (+ c a1 ...)</tt> into <tt>scale * c + scale * a1 + ...</tt>.
 */
ptr<const sum>
expand_prod_sum( const expr &scale, const sum  &addition )
{
  ASSERT( ! scale.is_a< sum >() );

  if( scale.is_numerical() )
  { // easy case -> sum::sca does the work
    const number &ns = scale.as_a< numerical >()->get();

    ptr<const sum> ret
      = ns.unit()
      ? &addition
      : sum::sca( ns, addition );

    ret->basic::expand();
    return ret;
  }

  // hard case : distribute term by term

  number coef = 0;

  ptr< const prod > scale_prod = scale.get()->as_prod();

  // addition.size() for nsp * addition
  // + 1 for scale_prod * addition.coef()
  container::ptr_unsafe_vector< const prod > seq ( addition.size() + 1 );

  { // coefficient handling
    const prod::handle ph = scale_prod.get();

    // cannot be numerical since [scale_prod] isn't
    seq.push_back( ph.sca( addition.coef() ).get() );
  }
  foreach( const prod* p, addition )
  {
    ASSERT( p );

    const expr ex = prod::mul( *p, *scale_prod );

    // seams reasonable here
    ASSERT( ! ex.is_a< sum >() );

    if( ex.is_numerical() )
      coef += ex.as_a< numerical >()->get();

    else
      seq.push_back( ex.get()->as_prod() );
  }

  ptr< sum > ret = sum::from_mutable_prod_range( coef, seq.begin(), seq.end() );
  ret->basic::expand(); // mark expanded
  return ret;
}

}} // namespace analysis::expand_detail
