// common includes
#include "analysis/expr.hpp"
#include "analysis/basic.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

#include "analysis/symbol.hpp"

#include "analysis/numeric.hpp"
#include "analysis/power.hpp"
#include "analysis/prod.hpp"
#include "analysis/sum.hpp"

#include "analysis/stdfunc/log.hpp"

using namespace analysis;

// numeric
expr numeric::differentiate(const symbol &) const
{ return number::zero(); }

// symbol
expr symbol_::differentiate(const symbol &s) const
{
  return symbol_::has(s) // means [this == s]
  ? number::one()
  : number::zero();
}

// power
namespace {

/*!\brief Logarithmic differentiation
 *
 * This method is used internally by \c prod::differentiate()
 * and \c power::differentiate().
 *
 * It returns the logarithmic derivative with respect to \c s,
 * ie. d( log(s) ) = d(s) / s.
 */
expr diff_log(
  const expr &base
, const expr &dbase
, const expr &expo
, const expr &dexpo
)
{
  expr ret = number::zero();

  if( ! dbase.null() )
    ret  = expo * dbase / base;

  if( ! dexpo.null() )
    ret += log( base ) * dexpo;

  return ret;
}

}

expr power::differentiate(const symbol &s) const
{
  const expr &db = m_base.diff(s);
  const expr &de = m_expo.diff(s);

  const bool bh = ! db.null();
  const bool eh = ! de.null();

  // trivial case
  if( !bh & !eh )
    return number::zero();

  // d(b^e) = e * d(b) * b^(e-1) when e is constant
  if( !eh )
    return m_expo * db
         * m_base.pow( m_expo - number::one() );

  // d(b^e) = b^e * dlog( b^e )
  return expr(this)
       * diff_log( m_base, db, m_expo, de );
}

// prod
namespace {

struct diff_log_f
: std::unary_function<const power*, expr>
{
  diff_log_f(const symbol* s)
  : sym(s) {}

  inline expr operator()( const power* p ) const
  {
    const expr &base = p->base();
    const expr &expo = p->expo();
    return diff_log(
      base, base.diff( *sym )
    , expo, expo.diff( *sym )
    );
  }

private:
  const symbol* sym;
};

}

expr prod::differentiate(const symbol &s) const
{
  const number &c = super::coef();

  // d(* c) -> 0
  // d(* 0 ...) -> 0
  if( empty() || c.null() )
    return number::zero();

  // d(* c x) -> (* c d(x))
  if( super::is_monomial() )
    return c * super::monomial()->power::differentiate(s);

  // We use logarithmic differentiation :
  //   this = product_i p_i
  // => log(this) = sum_i log(p_i)
  // => dlog(this) = d(this)/this = sum_i dlog(p_i)
  // => d(this) = this * sum_i dlog(p_i)

  const diff_log_f dlf ( &s );
  const expr &dl_sum =
    sum::from_expr_range(
      boost::make_transform_iterator( begin(), dlf )
    , boost::make_transform_iterator( end(),   dlf )
    );

  return dl_sum * expr(this);
}

// sum
expr sum::differentiate(const symbol &s) const
{
  return sum::from_expr_range(
    boost::make_transform_iterator( begin(), boost::bind( &prod::differentiate, _1, s ) )
  , boost::make_transform_iterator( end(),   boost::bind( &prod::differentiate, _1, s ) )
  );
}
