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
int fmpq_cmp_d(const fmpq_t z, double d)
{
  /* TODO */
  return 0;
}

int fmpq_cmpabs_ui(const fmpq_t z, ulong a)
{
  int ret = 0;
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
void fmpq_mul_2exp(fmpq_t q, const fmpq_t r, ulong e)
{
  _fmpq_mul_2exp( fmpq_numref(q), fmpq_denref(q), fmpq_numref(r), fmpq_denref(r), e );
}
void fmpq_div_2exp(fmpq_t q, const fmpq_t r, ulong e)
{
  _fmpq_mul_2exp( fmpq_denref(q), fmpq_numref(q), fmpq_denref(r), fmpq_numref(r), e );
}
