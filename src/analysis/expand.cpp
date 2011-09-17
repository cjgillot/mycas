#include "analysis/expr.hpp"
#include "analysis/basic.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

#include "util/move.hpp"
#include "util/assert.hpp"
#include "util/foreach.hpp"

#include "analysis/power.hpp"
#include "analysis/prod.hpp"
#include "analysis/sum.hpp"

#include "analysis/symbol.hpp"

#include "analysis/ptr.hpp"

#include "container/unsafe_vector.hpp"
#include "container/ptr_container.hpp"

using namespace analysis;

static ptr<const basic>   sum_expand(const sum   &);
static ptr<const basic>  prod_expand(const prod  &);
static ptr<const basic> power_expand(const power &);

expr   sum::expand() const { return   sum_expand( *this ); }
expr  prod::expand() const { return  prod_expand( *this ); }
expr power::expand() const { return power_expand( *this ); }


namespace {

/*!\brief Expand the product of two sums
 *
 * Complexity :
 * - time : n^2 * lg n
 * - space : n^2
 */
ptr<const sum> expand_sum_sum(
  const sum &a
, const sum &b
) {
  // a = (+ ac a1 a2 ...)
  // b = (+ bc b1 b2 ...)

  typedef container::ptr_unsafe_vector< const prod > pvector_t;

  ASSERT( ! a.empty() && ! b.empty() );

  // to be initialized later, at the end of next block
  ptr< sum > retp;

  { // sum by coefficient scaling
    const number &ac = a.coef();
    const number &bc = b.coef();

    // scaled products
    pvector_t scaled ( a.size() + b.size() );

    if( ! ac.null() )
    {
      if( ac.unit() )
        scaled.push_range( b.begin(), b.end() );
      else {
        util::scoped_ptr< sum > sp ( sum::sca( ac, b ) );
        scaled.push_range( sp->begin(), sp->end() );
      }
    }
    if( ! bc.null() )
    {
      if( bc.unit() )
        scaled.push_range( a.begin(), a.end() );
      else {
        util::scoped_ptr< sum > sp ( sum::sca( bc, a ) );
        scaled.push_range( sp->begin(), sp->end() );
      }
    }

    // create sum, n * lg n time
    retp.reset(
      sum::from_prod_range(
        ac * bc
      , scaled.begin()
      , scaled.end()
      )
    );
  }

  // hard multiplication work
  // commit into \c retp after each external loop
  // to allow cancellation, and prevent too much sequence growth
  foreach( const prod* pa, a )
  {
    ASSERT( pa );

    // discovered constants
    number coef ( number::zero() );
    // products : pa * b
    pvector_t seq ( b.size() );

    foreach( const prod* pb, b )
    {
      ASSERT( pb );

      const expr &p = prod::mul( *pa, *pb );

      // seams reasonable here...
      ASSERT( ! p.is_a< sum >() );

      if( p.is_numeric() )
        // flush constant
        coef += number( p.as_a< numeric >() );

      else
        seq.push_back( p.get()->as_prod() );
    }

    // commit the work
    util::scoped_ptr< sum > tmp ( sum::from_prod_range( coef, seq.begin(), seq.end() ) );
    retp.reset( sum::add( *retp, *tmp ) );
  }

  return retp;
}

/*!\brief Expand an expression of the form scale * addition
 *
 * Provided \c scale is not a sum, expand the expression
 * <tt>scale * (+ c a1 ...)</tt> into <tt>scale * c + scale * a1 + ...</tt>.
 */
ptr<const sum> distribute_over(
  const expr &scale
, const sum  &addition
) {
  ASSERT( ! scale.is_a< sum >() );

  if( scale.is_numeric() )
  { // easy case -> sum::sca does the work
    const number &ns = scale.as_a< numeric >();

    ptr<const sum> ret
      = ns.unit()
      ? &addition
      : sum::sca( ns, addition );

    ret->basic::expand();
    return ret;
  }

  // hard case : distribute term by term

  number coef = number::zero();

  ptr< const prod > scale_prod = scale.get()->as_prod();

  // addition.size() for nsp * addition
  // + 1 for scale_prod * addition.coef()
  container::ptr_unsafe_vector< const prod > seq ( addition.size() + 1 );

  { // coefficient handling
    ptr< const prod > cprod = prod::from_number( addition.coef() );

    // nsp * cprod cannot be numeric since cprod is and nsp isn't
    seq.push_back( prod::mul( *cprod, *scale_prod ) );
  }
  foreach( const prod* p, addition )
  {
    ASSERT( p );

    const expr &ex = prod::mul( *p, *scale_prod );

    // seams reasonable here
    ASSERT( ! ex.is_a< sum >() );

    if( ex.is_numeric() )
      coef += number( ex.as_a< numeric >() );

    else
      seq.push_back( ex.get()->as_prod() );
  }

  ptr< sum > ret = sum::from_prod_range( coef, seq.begin(), seq.end() );
  ret->basic::expand(); // mark expanded
  return ret;
}

}

// expairseq common code
namespace {

struct power_expander
: std::unary_function<const power*, ptr<const basic> >
{
  inline ptr<const basic> operator()( const power* ep ) const
  { return power_expand( *ep ); }
};

struct prod_expander
: std::unary_function<const prod*, ptr<const basic> >
{
  inline ptr<const basic> operator()( const prod* ep ) const
  { return prod_expand( *ep ); }
};

}

// power
namespace {

struct repower
: public std::unary_function<const expr&, expr>
{
  const expr* base;

  repower( const expr &b )
  : base( &b ) {}

  inline expr operator()( const expr &p ) const
  { return base->pow( p ).expand(); }
};

}

ptr<const basic> power_expand(const power &self)
{
  const expr &e_base = self.base().expand();
  const expr &e_expo = self.expo().expand();

  // (^ b (+ ec e1 e2 ...)) -> (^ b ec) * (^ b e1) * ...
  if( e_expo.is_a< sum >() )
  {
    const sum &s_expo = *e_expo.as_a< sum >();

    // (^ b ec)
    const expr &base_coef = e_base.pow( s_expo.coef() ).expand();

    // (^ b e1) * (^ b e2) * ...
    expr rest ( number::zero() );
    {
      container::unsafe_vector<expr> seq ( s_expo.size() );
      std::transform(
        s_expo.begin(), s_expo.end()
      , std::back_inserter( seq )
      , repower( e_base )
      );

      ptr< prod > restp =
        prod::from_expr_range( seq.begin(), seq.end() );

      // This expand is normally very cheap,
      // since all expressions in the range are
      // expanded powers (by repower).

      // However, it is useful when at least one of the
      // powers has been changed.
      rest = prod_expand( *restp );
    }

    // now, we shall build up base_coef * rest

    // asking for base_coef * rest does not work,
    // since the prod::mul behaviour is to sum exponents,
    // ie. back to square one

    if( rest.is_a< sum >() )
    {
      const sum &rest_sum = *rest.as_a< sum >();

      if( base_coef.is_a< sum >() )
        return expand_sum_sum( *base_coef.as_a< sum >(), rest_sum );

      return distribute_over( base_coef, rest_sum );
    }

    if( base_coef.is_a< sum >() )
      return distribute_over( rest, *base_coef.as_a< sum >() );

    const expr &ret = base_coef * rest;
    return ret.get()->basic::expand();
  }

  // TODO : treat case base^n

  self.basic::expand();
  return &self;
}

// prod
ptr<const basic> prod_expand(const prod &self)
{
  { // trivial case ?
    foreach( const power* p, self )
      if( ( ! p->expo().is_numeric() )
        | ( ! p->base().is_a< symbol_ >() ) )
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
        & ( p->base().is_a< sum >() ) )
        goto has_sum;

    return self.basic::expand();

  has_sum:
    children.reset( new exvec_t( self.begin(), self.end() ) );
  }

  ASSERT( ! children->empty() );

  if( children->size() == 1 )
  {
    const expr &ret = self.coef() * (*children)[0];

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

  return distribute_over( non_sum, last );
}

// sum
ptr<const basic> sum_expand(const sum &self)
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

  foreach( const prod* p, *retp )
    p->basic::expand();

  return retp->basic::expand();
}
