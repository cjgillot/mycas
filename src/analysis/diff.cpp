#include "analysis/expr.hpp"
#include "analysis/basic.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

using namespace analysis;

// numeric
#include "analysis/number.hpp"
#include "analysis/numeric.hpp"

expr numeric::diff(const symbol &, unsigned n) const
{ return n ? number::zero() : expr(this); }

// symbol
#include "analysis/symbol.hpp"

expr symbol_::diff(const symbol &s, unsigned n) const
{
  if( n == 0 )
    return expr(this);

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
    return expr(this);

  bool bh = m_base.has(s)
  ,    eh = m_expo.has(s);

  if( !bh & !eh )
    return number::zero();

  if( m_expo.unit() )
    return m_base.diff(s,n);

  if( n > 1 )
    return diff(s).diff(s, n-1);

  if( !eh )
    return m_expo * m_base.diff(s) * m_base.pow( m_expo - number::one() );

  return diff_log(s) * expr(this);
}

// prod
#include "analysis/prod.hpp"

expr prod::diff(const symbol &s, unsigned n) const
{
  if( n == 0 )
    return expr(this);

  //! now, n >= 1
  const number &c = super::coef();

  //! d^n(* c) -> 0
  if(super::is_empty())
    return number::zero();

  //! d^n(* 0 ...) -> 0
  if(c.null())
    return number::zero();

  //! d^n(* c x) -> (* c d^n(x))
  if( super::is_mono() )
  {
    expr b ( super::mono() );
    return expr(c) * b.diff(s, n);
  }

  if( n > 1 )
    return diff(s).diff(s, n-1);

  //! now, n == 1

  //! We use logarithmic differentiation :
  //!   \f[ e = \Pi_i p_i \f]
  //! \f[ ln(e) = \sum_i ln(p_i) \Rightarrow dln(e) = de/e = \sum_i dln(p_i) \f]
  //! so, \f[ de = e * \sum_i dln(p_i) \f]
  //!
  //! Since \c p_i is a \c power, and that power differentiation
  //! internally needs logarithm, \c epair { aka. \c power::handle }
  //! provides it gently.

  expr ret;

  foreach( const epair &e, *this )
    ret += e.diff_log(s);

  ret *= expr(this);
  return ret;
}

// sum
#include "analysis/sum.hpp"

expr sum::diff(const symbol &s, unsigned n) const {
  if( n == 0 )
    return expr(this);

  expr ret ( number::zero() );

  foreach( const epair &e, *this )
    ret += e.ptr()->diff( s, n );

  return ret;
}

