#include <fmpz.h>
#include <fmpq.h>

#include <mpfr.h>

/****** double support ******/

void fmpz_set_d(fmpz_t t, double d)
{
  mpz_t tmp;
  mpz_init(tmp);
  mpz_set_d(tmp, d);

  fmpz_set_mpz(t, tmp);

  mpz_clear(tmp);
}

void fmpq_set_d(fmpq_t t, double d)
{
  mpq_t tmp;
  mpq_init(tmp);
  mpq_set_d(tmp, d);

  fmpz_set_mpz(fmpq_numref(t), mpq_numref(tmp));
  fmpz_set_mpz(fmpq_denref(t), mpq_denref(tmp));

  mpq_clear(tmp);
}

double fmpz_get_d(const fmpz_t t)
{
  if( COEFF_IS_MPZ(*t) )
    return mpz_get_d( COEFF_TO_PTR(*t) );

  else
    return (double)*t;
}

double fmpq_get_d(fmpq_t t)
{
  return fmpz_get_d( fmpq_numref(t) ) / fmpz_get_d( fmpq_denref(t) );
}

/* comparison */
int fmpz_cmp_d(const fmpz_t z, double d)
{
  if( COEFF_IS_MPZ(*z) )
    return mpz_cmp_d( COEFF_TO_PTR(*z), d );
  else
    return ( *z < d ) ? -1 : ( *z > d );
}

int fmpq_cmpabs_ui(const fmpq_t z, ulong a)
{
  int ret;
  fmpz_t tmp = { 0 };

  fmpz_mul_ui(tmp, fmpq_denref(z), a);
  ret = fmpz_cmpabs( fmpq_numref(z), tmp );
  fmpz_clear(tmp);

  return ret;
}

int fmpq_cmp(const fmpq_t q, const fmpq_t r)
{
  int ret = 0;
  fmpz_t tmp = { 0 };

  fmpz_mul   ( tmp, fmpq_numref(q), fmpq_denref(r) );
  fmpz_submul( tmp, fmpq_denref(q), fmpq_numref(r) );

  ret = fmpz_sgn(tmp);

  fmpz_clear(tmp);

  return ret;
}

int fmpq_cmp_ui(const fmpq_t q, ulong a, ulong b)
{
  int ret = 0;
  fmpz_t tmp = { 0 };

  fmpz_mul_ui   ( tmp, fmpq_numref(q), b );
  fmpz_submul_ui( tmp, fmpq_denref(q), a );

  ret = fmpz_sgn(tmp);

  fmpz_clear(tmp);

  return ret;
}

int fmpq_cmp_si(const fmpq_t q, long a, ulong b)
{
  int ret = 0;
  fmpz_t tmp = { 0 };

  fmpz_mul_ui   ( tmp, fmpq_numref(q), b );
  if( a >= 0 ) fmpz_submul_ui( tmp, fmpq_denref(q), a );
  else         fmpz_addmul_ui( tmp, fmpq_denref(q),-a );

  ret = fmpz_sgn(tmp);

  fmpz_clear(tmp);

  return ret;
}

/* set */
/* int fmpz_set_str(fmpz_t, const char*, int); */
int fmpq_set_str(fmpq_t q, const char* s, int b)
{
  int ret = 0;

  mpq_t mp;
  mpq_init(mp);
  ret = mpq_set_str(mp, s, b);

  fmpq_set_mpq(q, mp);

  mpq_clear(mp);

  return ret;
}

void fmpz_set_q(fmpz_t z, const fmpq_t q)
{
  fmpz_tdiv_q( z, fmpq_numref(q), fmpq_denref(q) );
}

void mpfr_set_fz(mpfr_ptr f, const fmpz_t z, mpfr_rnd_t rnd)
{
  if( COEFF_IS_MPZ(*z) )
    mpfr_set_z(f, COEFF_TO_PTR(*z), rnd);
  else
    mpfr_set_si(f, *z, rnd );
}
void mpfr_set_fq(mpfr_ptr f, const fmpq_t q, mpfr_rnd_t rnd)
{
  mpfr_set_fz( f, fmpq_numref(q), rnd );

  if( COEFF_IS_MPZ(*fmpq_denref(q)) )
    mpfr_div_z(f, f, COEFF_TO_PTR(*fmpq_denref(q)), rnd);
  else
    mpfr_div_si(f, f, *fmpq_denref(q), rnd );
}

/* 2exp */
#if 1

static void _fmpq_mul_2exp(fmpz_t qn, fmpz_t qd, const fmpz_t rn, const fmpz_t rd, ulong e)
{
  /* FIXME : overflow ? */
  fmpz_t two = { 2 };
  ulong de = fmpz_remove( qd, rd, two );

  if( de <= e )
    fmpz_mul_2exp( qn, rn, e - de );

  else
  {
    fmpz_set( qn, rn );
    fmpz_mul_2exp( qd, rd, de - e );
  }
}

#else

#include "mpir.h"
#include "mpir/gmp-impl.h"
#include "mpir/longlong.h"

#define mpq_mul_2exp  static mpq_dummy_mul_2e
#define mpq_div_2exp  static mpq_dummy_div_2e

#include "mpir/mpq/md_2exp.c"

static void _fmpq_mul_2exp(fmpz_t qn, fmpz_t qd, const fmpz_t rn, const fmpz_t rd, ulong e)
{
  ulong plow = 0;

  mpz_t rn_tmp; mpz_srcptr rn_p = 0; int rn_set = 0;

  mpq_t qn_tmp; mpz_ptr qn_p = 0; int qn_set = 0;
  mpq_t qd_tmp; mpz_ptr qd_p = 0; int qd_set = 0;

  if( ! COEFF_IS_MPZ( *rd ) )
  {
    count_trailing_zeros( rd, plow )

    if( plow < e )
    {
      qd = rd >> plow;
      fmpz_mul_2exp( qn, rn, e - plow );
    }
    else
    {
      qd = rd >> e;
      if( qn != rn )
        fmpz_set( qn, rn );
    }

    return;
  }

#define BIND( x )                   \
  if( COEFF_IS_MPZ( *x ) )          \
    x##_p = COEFF_TO_PTR( *x );     \
  else                              \
  {                                 \
    mpz_init_set_si( x##_tmp, *x ); \
    x##_set = 1;                    \
  }

  BIND( rn ) BIND( qn ) BIND( qd )

#undef BIND

  mord_2exp( qn_p, qd_p, rn_p, COEFF_TO_PTR( *qd ), e );

#define UNBIND( x )       \
  if( x##_set )           \
    mpz_clear( x##_tmp );

  UNBIND( rn )

#undef UNBIND

#define UNBIND( x )               \
  if( x##_set )                   \
  {                               \
    fmpz_set_mpz( *x, x##_tmp );  \
    mpz_clear( x##_tmp );         \
  }

  UNBIND( qn ) UNBIND( qd )

#undef UNBIND
}

#endif

void fmpq_mul_2exp(fmpq_t q, const fmpq_t r, ulong e)
{
  _fmpq_mul_2exp( fmpq_numref(q), fmpq_denref(q), fmpq_numref(r), fmpq_denref(r), e );
}
void fmpq_div_2exp(fmpq_t q, const fmpq_t r, ulong e)
{
  _fmpq_mul_2exp( fmpq_denref(q), fmpq_numref(q), fmpq_denref(r), fmpq_numref(r), e );
}

/* bitwise */
void fmpz_com(fmpz_t r, const fmpz_t a)
{
  if( COEFF_IS_MPZ( *a ) )
  {
    mpz_com( _fmpz_promote( r ), COEFF_TO_PTR( *a ) );
    _fmpz_demote_val( r );
  }

  else
    fmpz_set_ui( r, ~ *a );
}

void fmpz_and(fmpz_t r, const fmpz_t a, const fmpz_t b)
{
  mpz_t tmp;

  if( COEFF_IS_MPZ( *a ) )
  {
    if( COEFF_IS_MPZ( *b ) )
    {
      mpz_and( _fmpz_promote( r ), COEFF_TO_PTR( *a ), COEFF_TO_PTR( *b ) );
      _fmpz_demote_val( r );
    }

    else {
      mpz_set_ui( tmp, *b );
      mpz_and( _fmpz_promote( r ), COEFF_TO_PTR( *a ), tmp );
      mpz_clear( tmp );
      _fmpz_demote_val( r );
    }
  }

  else
  {
    if( COEFF_IS_MPZ( *a ) )
    {
      mpz_set_ui( tmp, *a );
      mpz_and( _fmpz_promote( r ), tmp, COEFF_TO_PTR( *b ) );
      mpz_clear( tmp );
      _fmpz_demote_val( r );
    }

    else
      fmpz_set_ui( r, *a & *b );
  }
}

void fmpz_ior(fmpz_t r, const fmpz_t a, const fmpz_t b)
{
  mpz_t tmp;

  if( COEFF_IS_MPZ( *a ) )
  {
    if( COEFF_IS_MPZ( *b ) )
    {
      mpz_ior( _fmpz_promote( r ), COEFF_TO_PTR( *a ), COEFF_TO_PTR( *b ) );
      _fmpz_demote_val( r );
    }

    else {
      mpz_set_ui( tmp, *b );
      mpz_ior( _fmpz_promote( r ), COEFF_TO_PTR( *a ), tmp );
      mpz_clear( tmp );
      _fmpz_demote_val( r );
    }
  }

  else
  {
    if( COEFF_IS_MPZ( *a ) )
    {
      mpz_set_ui( tmp, *a );
      mpz_ior( _fmpz_promote( r ), tmp, COEFF_TO_PTR( *b ) );
      mpz_clear( tmp );
      _fmpz_demote_val( r );
    }

    else
      fmpz_set_ui( r, *a | *b );
  }
}

void fmpz_xor(fmpz_t r, const fmpz_t a, const fmpz_t b)
{
  mpz_t tmp;

  if( COEFF_IS_MPZ( *a ) )
  {
    if( COEFF_IS_MPZ( *b ) )
    {
      mpz_xor( _fmpz_promote( r ), COEFF_TO_PTR( *a ), COEFF_TO_PTR( *b ) );
      _fmpz_demote_val( r );
    }

    else {
      mpz_set_ui( tmp, *b );
      mpz_xor( _fmpz_promote( r ), COEFF_TO_PTR( *a ), tmp );
      mpz_clear( tmp );
      _fmpz_demote_val( r );
    }
  }

  else
  {
    if( COEFF_IS_MPZ( *a ) )
    {
      mpz_set_ui( tmp, *a );
      mpz_xor( _fmpz_promote( r ), tmp, COEFF_TO_PTR( *b ) );
      mpz_clear( tmp );
      _fmpz_demote_val( r );
    }

    else
      fmpz_set_ui( r, *a ^ *b );
  }
}
