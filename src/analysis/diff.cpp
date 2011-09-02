#include "analysis/expr.hpp"
#include "analysis/basic.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

#include "util/assert.hpp"

#include "analysis/number.hpp"
#include "analysis/numeric.hpp"
#include "analysis/symbol.hpp"

#include "analysis/power.hpp"
#include "analysis/prod.hpp"
#include "analysis/sum.hpp"

#include "analysis/stdfunc/log.hpp"

using namespace analysis;

// numeric
expr numeric::diff(const symbol &, unsigned n) const
{ return n ? number::zero() : this; }

// symbol
expr symbol_::diff(const symbol &s, unsigned n) const
{
  if( n == 0 )
    return this;

  if( n == 1 && symbol_::has(s) )
    return number::one();

  return number::zero();
}

// power
namespace {

/*!\brief Logarithmic differentiation
 *
 * This method is used internally by \c prod::diff()
 * and \c power::diff().
 *
 * It returns the logarithmic derivative with respect to \c s.
 */
expr diff_log(const expr &base, const expr &expo, const symbol &s)
{
  const expr &dbase = base.diff(s);
  const expr &dexpo = expo.diff(s);

  expr ret = number::zero();

  if( ! dbase.null() )
    ret  = expo * dbase / base;

  if( ! dexpo.null() )
    ret += log( base ) * dexpo;

  return ret;
}

struct diff_log_f
: std::unary_function<const power*, expr>
{
  diff_log_f( const symbol &s )
  : sym( &s ) {}

  inline expr operator()( const power* p ) const
  { return diff_log( p->base(), p->expo(), *sym ); }

private:
  const symbol* sym;
};

}

expr power::diff(const symbol &s, unsigned n) const
{
  if( n == 0 )
    return this;

  bool bh = m_base.has(s)
  ,    eh = m_expo.has(s);

  // trivial case
  if( !bh & !eh )
    return number::zero();

  // this cases should not happen
  // since eval() simplifies it
#if 0
  if( m_expo.null() )
    return number::zero();

  if( m_expo.unit() )
    return m_base.diff(s,n);
#else
  ASSERT_MSG( ! m_expo.null() && ! m_expo.unit(), "Differentiating unevaluated power" );
#endif

  // no
  if( n > 1 )
    return power::diff(s, 1).diff(s, n-1);

  if( !eh )
    return m_expo * m_base.diff(s) * m_base.pow( m_expo - number::one() );

  return diff_log( m_base, m_expo, s ) * expr( this );
}

// prod
expr prod::diff(const symbol &s, unsigned n) const
{
  if( n == 0 )
    return this;

  //! now, n >= 1
  const number &c = super::coef();

  //! d^n(* c) -> 0
  //! d^n(* 0 ...) -> 0
  if( empty() || c.null() )
    return number::zero();

  //! d^n(* c x) -> (* c d^n(x))
  if( super::is_monomial() )
  {
    const expr &b = super::monomial();
    return expr(c) * b.diff(s, n);
  }

  if( n > 1 )
    return diff(s, 1).diff(s, --n);

  //! now, n == 1

  //! We use logarithmic differentiation :
  //!   \f[ e = \Pi_i p_i \f]
  //! \f[ ln(e) = \sum_i ln(p_i) \Rightarrow dln(e) = de/e = \sum_i dln(p_i) \f]
  //! so, \f[ de = e * \sum_i dln(p_i) \f]
  //!
  //! Since \c p_i is a \c power, and that power differentiation
  //! internally needs logarithm, \c epair { aka. \c power::handle }
  //! provides it gently.

  const expr &diff_log_sum =
    sum::from_expr_range(
      boost::make_transform_iterator( begin(), diff_log_f( s ) )
    , boost::make_transform_iterator( end(),   diff_log_f( s ) )
    );

  return diff_log_sum * expr( this );
}

// sum
expr sum::diff(const symbol &s, unsigned n) const {
  if( n == 0 )
    return this;

  return sum::from_expr_range(
    boost::make_transform_iterator( begin(), boost::bind( &prod::diff, _1, s, n ) )
  , boost::make_transform_iterator( end(),   boost::bind( &prod::diff, _1, s, n ) )
  );
}

