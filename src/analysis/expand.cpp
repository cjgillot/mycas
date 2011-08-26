#include "analysis/expr.hpp"
#include "analysis/basic.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

#include "util/move.hpp"
#include "util/assert.hpp"
#include "util/foreach.hpp"

using namespace analysis;

// basic
expr basic::expand() const
{ return this; }

// power
#include "analysis/power.hpp"
#include "analysis/prod.hpp"
#include "analysis/sum.hpp"

namespace {

expr expand_sum_num(
  const sum &s
, const number &n
);

}

expr power::expand() const
{
  const expr &e_base = base().expand();
  const expr &e_expo = expo().expand();

  // (^ b (+ ec e1 e2 ...)) -> (* (^ b ec) (^ b e1) ...)
  if( e_expo.is_a< sum >() )
  {
    const sum &s_expo = *e_expo.as_a< sum >();

    container::unsafe_vector<expr> seq ( 1 + s_expo.size() );

    {
      const expr &base_coef = e_base.pow( s_expo.coef() );
      base_coef.eval();
      seq.push_back( base_coef );
    }
    foreach( const prod* p, s_expo )
    {
      ASSERT( p );

      expr ex ( e_base.pow( p ) );
      ex.eval();
      seq.push_back( ex );
    }

    const expr &ret = prod::from_expr_range( seq.begin(), seq.end() );
    return ret.expand();
  }

  // TODO : treat case base^n

  return this;
}

// expairseq common code
namespace {

template< class Mono >
struct expander
: std::unary_function<expr, const Mono*>
{
  inline expr operator()( const Mono* ep ) const
  { return ep->expand(); }
};

}

// prod
#include "container/unsafe_vector.hpp"

namespace {

expr expand_sum_sum(
  const sum &a
, const sum &b
) {
  // a = (+ ac a1 a2 ...)
  // b = (+ bc b1 b2 ...)

  ASSERT( ! a.empty() && ! b.empty() );

  // to be initialized later
  boost::intrusive_ptr< sum > retp;

  { // sum by coefficient scaling
    const number &ac = a.coef();
    const number &bc = b.coef();

    number coef ( ac * bc );

    // coefficient scaling
    container::ptr_unsafe_vector< const prod > scaled ( a.size() + b.size() + a.size() * b.size() );

    if( ! ac.null() )
    {
      if( ac.unit() )
        scaled.push_range( b.begin(), b.end() );
      else {
        boost::intrusive_ptr< sum > sp ( sum::sca( ac, b ) );
        scaled.push_range( sp->begin(), sp->end() );
      }
    }
    if( ! bc.null() )
    {
      if( bc.unit() )
        scaled.push_range( a.begin(), a.end() );
      else {
        boost::intrusive_ptr< sum > sp ( sum::sca( bc, a ) );
        scaled.push_range( sp->begin(), sp->end() );
      }
    }

    // create sum
    retp.reset( sum::from_prod_range( coef, scaled.begin(), scaled.end() ) );
  }

  // hard multiplication work
  // commit into \c retp after each external loop
  // to allow cancellation
  foreach( const prod* pa, a )
  {
    // discovered constants
    number coef ( number::one() );
    // products : ai * b
    container::ptr_unsafe_vector< const prod > seq ( b.size() );

    foreach( const prod* pb, b )
    {
      expr p ( prod::mul( *pa, *pb ) );
      p.eval();

      // seams reasonable here...
      ASSERT( ! p.is_a< sum >() );

      if( p.is_numeric() )
        // flush constants
        coef *= number( p.as_a< numeric >() );

      else
        seq.push_back( p.get()->as_prod() );
    }

    // commit the work
    boost::intrusive_ptr< sum > tmp ( sum::from_prod_range( coef, seq.begin(), seq.end() ) );
    retp.reset( sum::add( *retp, *tmp ) );
  }

  return retp.get();
}

}

expr prod::expand() const
{
  typedef std::vector< expr > exvec_t;
  util::move_ptr< exvec_t > children ( super::map_children( expander< power >() ) );

  // nothing has changed
  if( ! children )
    return this;

  ASSERT( ! children->empty() );

  // we have to expand a variadic product of variadic sums

  // store the product of those which aren't sums
  //! \invariant \c non_sum is never a sum
  expr non_sum ( coef() );

  // store the current expansion state
  expr last_sum ( number::one() );

  foreach( const expr &ex, *children )
  {
    ex.eval();

    if( ! ex.is_a< sum >() )
    {
      non_sum *= ex;
      continue;
    }

    // got a sum
    if( ! last_sum.is_a< sum >() )
    {
      if( ! last_sum.unit() )
        non_sum *= last_sum;
      last_sum = ex;
      continue;
    }

    // got a product of sums (last_sum * ex)
    last_sum = expand_sum_sum(
      *last_sum.as_a< sum >()
    , *ex.as_a< sum >()
    );
  }

  if( ! last_sum.is_a< sum >() )
    return non_sum * last_sum;

  const sum &last = *last_sum.as_a< sum >();

  if( non_sum.is_numeric() )
  {
    const number &ns = non_sum.as_a< numeric >();
    return sum::sca( ns, last );
  }

  number cnst ( number::zero() );

  // last.size() for nsp * last
  // + 1 for nsp * coef()
  container::ptr_unsafe_vector< const prod > seq ( last.size() + 1 );

  boost::intrusive_ptr< const prod > nspp = non_sum.get()->as_prod();
  const prod &nsp = *nspp;

  { // coefficient handling
    boost::intrusive_ptr< const prod >
      cprod ( prod::from_numeric( coef().get() ) );

    // nsp * cprod cannot be numeric since cprod is and nsp isn't
    seq.push_back( prod::mul( *cprod, nsp ) );
  }
  foreach( const prod* p, last )
  {
    ASSERT( p );

    expr ex ( prod::mul( *p, nsp ) );
    ex.eval();

    ASSERT( ! ex.is_a< sum >() );

    if( ex.is_numeric() )
      cnst += number( ex.as_a< numeric >() );

    else
      seq.push_back( ex.get()->as_prod() );
  }

  return sum::from_prod_range( cnst, seq.begin(), seq.end() );
}

// sum
namespace {

/*!
 * \brief Construct a vector from a range of \c expr
 *
 * \param flat a reference to which append the result
 * \param coef a reference to a number to which add the found coefficients
 * \param rng a non-empty \c expr vector
 *
 * \post <tt>flat.size() >= rng.size()</tt>
 *
 * \warning The rng range is not preserved
 */
inline void
do_flatten_sum(
  number &coef
, std::vector< const prod* > &flat
, std::vector< expr > &rng
) {
  ASSERT( ! rng.empty() );

  // least size : no sum
  flat.reserve( flat.size() + rng.size() );

  foreach( const expr &e, rng )
    if( e.is_a< sum >() )
    { // found a sum -> flatten it
      const sum &s = *e.as_a< sum >();
      coef += s.coef();
      foreach( const prod* ep, s )
        flat.push_back( ep );
    }

    else if( e.is_numeric() )
      // found a number
      coef += number( e.as_a< numeric >() );

    else
      // neither a sum or a number
      flat.push_back( e.get()->as_prod() );
}

}

expr sum::expand() const
{
  typedef std::vector< expr > exvec_t;
  util::move_ptr< exvec_t > children ( super::map_children( expander< prod >() ) );

  // nothing has changed
  if( ! children )
    return this;

  // the sum was not degenerate
  ASSERT( ! children->empty() );

  // at least one has changed
  sum* retp = sum::from_expr_range( children->begin(), children->end() );
  retp->coef() += coef();

  const expr &ret ( retp );
  ret.eval();
  return ret;
}
