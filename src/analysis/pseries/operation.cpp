#include "analysis/pseries/pseries.hpp"
#include "analysis/pseries/repr.hpp"

#include "analysis/expr.hpp"

using namespace analysis;
using namespace pseries_detail;

#include "util/move.hpp"

#define UNARY_OP( name, func )                    \
pseries* pseries::name##_series(                  \
  const pseries* a                                \
) {                                               \
  util::scoped_ptr<pseries> ret                   \
    ( new pseries ( a->m_var ) );                 \
  ret->m_rep->m_value = pseries_export_##func(    \
    a->m_rep->m_value                             \
  );                                              \
  return ret.release();                           \
}

#define BINARY_OP( name, func )                   \
pseries* pseries::name##_series(                  \
  const pseries* a, const pseries* b              \
) {                                               \
  assert( a->m_var == b->m_var );                 \
  util::scoped_ptr<pseries> ret                   \
    ( new pseries( a->m_var ) );                  \
  ret->m_rep->m_value = pseries_export_##func(    \
    a->m_rep->m_value                             \
  , b->m_rep->m_value                             \
  );                                              \
  return ret.release();                           \
}

UNARY_OP( neg, neg )
UNARY_OP( inv, inv )

BINARY_OP( add, add )
BINARY_OP( sub, sub )

BINARY_OP( mul, mul )
BINARY_OP( div, div )

pseries* pseries::intpow(const pseries* b, unsigned long e)
{
  if( e == 0 )
    return new pseries( b->m_var, 1 );

  util::scoped_ptr< pseries > acc ( b->clone() );

  while( ! (e & 1) )
  {
    acc.reset( mul_series( acc.get(), acc.get() ) );
    e >>= 1;
  }

  if( e == 1 )
    return acc.release();

  util::scoped_ptr< pseries > ret ( acc.release() );

  acc.reset( mul_series( ret.get(), ret.get() ) );
  e >>= 1;

  while( e != 0 )
  {
    if( e & 1 )
      ret.reset( mul_series( acc.get(), ret.get() ) );

    acc.reset( mul_series( acc.get(), acc.get() ) );
    e >>= 1;
  }

  return ret.release();
}

#include "analysis/caml/expr_wrap.hpp"

pseries* pseries::sca_series(const expr &sca, const pseries* a)
{
  util::scoped_ptr<pseries> ret
    ( new pseries ( a->m_var ) );
  ret->m_rep->m_value = pseries_export_sca( expr_copy(sca), a->m_rep->m_value );
  return ret.release();
}

#include "analysis/vectorseq.hpp"

#include "util/foreach.hpp"

expr sum::series(const symbol &s) const
{
  ptr< pseries > ret = new pseries( s, coef() );

  foreach( const prod* h, *this )
  {
    ptr< const pseries > hs = h->series(s).as_a< pseries >();
    ret = pseries::add_series( ret.get(), hs.get() );
  }

  return ret;
}

expr prod::series(const symbol &s) const
{
  ptr< pseries > ret = new pseries( s, coef() );

  typedef std::pair<expr, long> pair_t;

  std::list< const power* > others;
  std::list< pair_t > posdeg, negdeg;

  ptr< pseries > posser = new pseries( s, 1 );
  ptr< pseries > negser = posser.get();

  foreach( const power* h, *this )
  {
    if( !h->expo().is_numerical() )
    {
      others.push_back(h);
      continue;
    }
    number n = h->expo().as_a<numerical>()->get();
    if( !n.fits_slong_p() )
    {
      others.push_back(h);
      continue;
    }

    long ne = n.get_slong();
    if( ne >= 0 )
      posdeg.push_back( std::make_pair( h->base(), ne ) );
    else
      negdeg.push_back( std::make_pair( h->base(),-ne ) );
  }

  foreach( const pair_t &p, posdeg )
  {
    ptr<const pseries> ser = p.first.series(s).as_a<pseries>();
    if( p.second > 1 )
      ser = pseries::intpow( ser.get(), p.second );
    posser = pseries::mul_series( posser.get(), ser.get() );
  }

  foreach( const pair_t &p, negdeg )
  {
    ptr<const pseries> ser = p.first.series(s).as_a<pseries>();
    if( p.second > 1 )
      ser = pseries::intpow( ser.get(), p.second );
    negser = pseries::mul_series( negser.get(), ser.get() );
  }

  foreach( const power* p, others )
  {
    ptr<const pseries> ser = p->series(s).as_a<pseries>();
    ret = pseries::mul_series( ret.get(), ser.get() );
  }

  ret = pseries::mul_series( ret.get(), posser.get() );
  ret = pseries::div_series( ret.get(), negser.get() );

  return ret;
}
