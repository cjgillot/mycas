#include "analysis/expr.hpp"
#include "analysis/basic.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

#include "util/assert.hpp"

using namespace analysis;

// numeric
#include "analysis/number.hpp"
#include "analysis/numeric.hpp"

expr numeric::diff(const symbol &, unsigned n) const
{ return n ? number::zero() : this; }

// symbol
#include "analysis/symbol.hpp"

expr symbol_::diff(const symbol &s, unsigned n) const
{
  if( n == 0 )
    return this;

  if( n == 1 && symbol_::has(s) )
    return number::one();

  return number::zero();
}

// power
#include "analysis/power.hpp"
#include "analysis/stdfunc/log.hpp"

expr power::diff_log(const symbol &s) const
{
  expr ret ( number::zero() );

  if( m_base.has(s) )
    ret  = m_expo * m_base.diff(s) / m_base;

  if( m_expo.has(s) )
    ret += log( m_base ) * m_expo.diff(s);

  return ret;
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

  return diff_log(s) * expr( this );
}

// prod
#include "analysis/prod.hpp"

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
  if( super::is_mono() )
  {
    expr b ( super::mono() );
    return expr(c) * b.diff(s, n);
  }

  if( n > 1 )
    return diff(s, 1).diff(s, n-1);

  //! now, n == 1

  //! We use logarithmic differentiation :
  //!   \f[ e = \Pi_i p_i \f]
  //! \f[ ln(e) = \sum_i ln(p_i) \Rightarrow dln(e) = de/e = \sum_i dln(p_i) \f]
  //! so, \f[ de = e * \sum_i dln(p_i) \f]
  //!
  //! Since \c p_i is a \c power, and that power differentiation
  //! internally needs logarithm, \c epair { aka. \c power::handle }
  //! provides it gently.

  expr ret ( number::zero() );

  foreach( const power* p, *this )
    ret += p->diff_log(s);

  ret *= expr(this);
  return ret;
}

// sum
#include "analysis/sum.hpp"

expr sum::diff(const symbol &s, unsigned n) const {
  if( n == 0 )
    return this;

  expr ret ( number::zero() );

  foreach( const prod* p, *this )
    ret += p->diff( s, n );

  return ret;
}

