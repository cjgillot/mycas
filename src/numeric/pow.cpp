#include "number.hpp"


using namespace numeric;
using detail::impl_t;
using detail::wrapper;

static void
fmpz_init_pow_uz( fmpz_t n, const fmpz_t b, const fmpz_t e )
{
  if( fmpz_is_one( b ) )
  {
    fmpz_init_set_ui( n, 1 );
    return;
  }

  if( fmpz_is_zero( b ) )
  {
    ASSERT( fmpz_sgn( e ) >= 0 );

    if( fmpz_is_zero( e ) )
      fmpz_init_set_ui( n, 1 );
    else
      fmpz_init_set_ui( n, 0 );

    return;
  }

  ASSERT( fmpz_abs_fits_ui( e ) );

  fmpz_init( n );
  fmpz_pow_ui( n, b, fmpz_get_ui( e ) );
}

static bool
fmpz_init_pow_z( fmpq_t r, const fmpz_t b, const fmpz_t e )
{
  if( fmpz_sgn( e ) < 0 )
  {
    fmpz_init_set_ui( fmpq_numref( r ), 1 );
    fmpz_init_pow_uz( fmpq_denref( r ), b, e );

    if( fmpz_sgn( fmpq_denref( r ) ) < 0 )
    {
      fmpz_set_si( fmpq_numref( r ), -1 );
      fmpz_abs( fmpq_denref( r ), fmpq_denref( r ) );
    }

    return true;
  }

  fmpz_init_pow_uz( fmpq_numref( r ), b, e );
  return false;
}

static void
fmpq_init_pow_z( fmpq_t r, const fmpq_t b, const fmpz_t e )
{
  if( fmpz_sgn( e ) < 0 )
  {
    fmpq_init( r );

    fmpz_init_pow_uz( fmpq_numref( r ), fmpq_denref( b ), e );
    fmpz_init_pow_uz( fmpq_denref( r ), fmpq_numref( b ), e );

    if( fmpz_sgn( fmpq_denref( r ) ) < 0 )
    {
      fmpz_neg( fmpq_numref( r ), fmpq_numref( r ) );
      fmpz_abs( fmpq_denref( r ), fmpq_denref( r ) );
    }

    return;
  }

  fmpq_init( r );

  fmpz_init_pow_uz( fmpq_numref( r ), fmpq_numref( b ), e );
  fmpz_init_pow_uz( fmpq_denref( r ), fmpq_denref( b ), e );

  return;
}

namespace {

struct pow_visitor
: boost::static_visitor<>
{
  impl_t* impl;

  explicit pow_visitor( impl_t &i )
  : impl( &i ) {}

  void operator()(const wrapper<mpfr_t> &a, const wrapper<mpfr_t> &b) const
  { *impl = pow( *a, *b ); }

  template<class T>
  void operator()(const T &a, const wrapper<mpfr_t> &b) const
  { *impl = pow( mpfr_class( *a ), *b ); }

  template<class T>
  void operator()(const T &a, const wrapper<fmpq_t> &b) const
  { *impl = pow( mpfr_class( *a ), *b ); }

  void operator()(const wrapper<mpfr_t> &a, const wrapper<fmpq_t> &b) const
  { *impl = pow( *a, mpfr_class( *b ) ); }

  template<class T>
  void operator()(const wrapper<mpfr_t> &a, const T &b) const
  { *impl = pow( *a, mpfr_class( *b ) ); }

  void operator()(const wrapper<fmpz_t> &a, const wrapper<fmpz_t> &b) const
  {
    fmpq_t ret;
    bool rat = fmpz_init_pow_z( ret, a->get_fmpz_t(), b->get_fmpz_t() );
    if( rat )
    {
      *impl = fmpq_class();
      fmpq* p = boost::get< wrapper<fmpq_t> >( *impl )->get_fmpq_t();
      fmpq_swap( p, ret );
      fmpq_clear( ret );
    }
    else // integer
    {
      *impl = fmpz_class();
      fmpz* p = boost::get< wrapper<fmpz_t> >( *impl )->get_fmpz_t();
      fmpz_swap( p, fmpq_numref( ret ) );
      fmpq_clear( ret );
    }
  }

  void operator()(const wrapper<fmpq_t> &a, const wrapper<fmpz_t> &b) const
  {
    fmpq_t ret;
    fmpq_init_pow_z( ret, a->get_fmpq_t(), b->get_fmpz_t() );

    if( fmpz_is_one( fmpq_denref( ret ) ) )
    {
      *impl = fmpz_class();
      fmpz* p = boost::get< wrapper<fmpz_t> >( *impl )->get_fmpz_t();
      fmpz_swap( p, fmpq_numref( ret ) );
      fmpq_clear( ret );
    }
    else
    {
      *impl = fmpq_class();
      fmpq* p = boost::get< wrapper<fmpq_t> >( *impl )->get_fmpq_t();
      fmpq_swap( p, ret );
      fmpq_clear( ret );
    }
  }
};

}

#if 1
number number::pow(const number &e) const
{
  number ret;

  boost::apply_visitor( pow_visitor( ret.m_impl ), m_impl, e.m_impl );

  return ret;
}
#else


number number::pow(const number &e) const
{
  uninitialized_t ut;
  number ret ( ut );

  mpfr_t bf, ef; bool bfc = false, efc = false;
  const __mpfr_struct *bfp = bf, *efp = ef;

  if( e.m_tag == int_tag && fmpz_is_zero( e.m_impl.z ) )
  {
    fmpz_init_set_ui( ret.m_impl.z, 1 );
    ret.m_tag = int_tag;
    goto ret;
  }

  switch( COMBINE( m_tag, e.m_tag ) )
  {
  case ii_tag:
    ret.m_tag = fmpz_init_pow_z( ret.m_impl.q, m_impl.z, e.m_impl.z );
    goto ret;

  case qi_tag:
    fmpq_init_pow_z( ret.m_impl.q, m_impl.q, e.m_impl.z );

    ret.m_tag = rat_tag;
    if( fmpz_is_one( fmpq_denref( ret.m_impl.q ) ) )
    {
      ASSERT( ret.m_impl.z == fmpq_numref( ret.m_impl.q ) );
      fmpz_clear( fmpq_denref( ret.m_impl.q ) );
      ret.m_tag = int_tag;
    }
    goto ret;

  case iq_tag:
    mpfr_init( bf ); mpfr_set_fz  ( bf,   m_impl.z, GMP_RNDN );
    mpfr_init( ef ); fmpq_get_mpfr( ef, e.m_impl.q, GMP_RNDN );
    bfc = efc = true;
    break;

  case qq_tag:
    mpfr_init( bf ); fmpq_get_mpfr( bf,   m_impl.q, GMP_RNDN );
    mpfr_init( ef ); fmpq_get_mpfr( ef, e.m_impl.q, GMP_RNDN );
    bfc = efc = true;
    break;

  case qf_tag:
    mpfr_init( bf ); fmpq_get_mpfr( bf,   m_impl.q, GMP_RNDN ); bfc = true;
    efp = e.m_impl.f;
    break;

  case fq_tag:
    bfp = m_impl.f;
    mpfr_init( ef ); fmpq_get_mpfr( ef, e.m_impl.q, GMP_RNDN ); efc = true;
    break;

  case ff_tag:
    bfp =   m_impl.f;
    efp = e.m_impl.f;
    break;

  case fi_tag:
    bfp = m_impl.f;
    mpfr_init( ef ); mpfr_set_fz  ( ef, e.m_impl.z, GMP_RNDN ); efc = true;
    break;

  case if_tag:
    mpfr_init( bf ); mpfr_set_fz  ( bf,   m_impl.z, GMP_RNDN ); bfc = true;
    efp = e.m_impl.f;
    break;

  default: ASSERT( false ); return 0;
  }

  mpfr_init( ret.m_impl.f );
  mpfr_pow( ret.m_impl.f, bfp, efp, GMP_RNDN );

  if( bfc ) mpfr_clear( bf );
  if( efc ) mpfr_clear( ef );

  ret.m_tag = flt_tag;

ret:
  return ret;
}

#endif
