#include <sys/types.h>
#include <limits.h>

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

/* hash */

#define HASH_BITS (8 * sizeof(size_t))
#define ROT_BITS ((HASH_BITS / 5) + 2) /* pure random choice */

#define HASH_MODULUS (~((size_t)0))
#define HASH_PINF  314159
#define HASH_MINF -HASH_PINF
#define HASH_NAN  0

#define rotate( xx, bits )  \
  ( ( xx << bits ) | ( xx >> ( HASH_BITS - bits ) ) )

size_t fmpz_hash(const fmpz_t z)
{
  __mpz_struct* mpz;
  size_t hash;

  if( !COEFF_IS_MPZ( *z ) )
    return *z;

  mpz = COEFF_TO_PTR( *z );
  hash = mpn_mod_1(mpz->_mp_d, mpz_size(mpz), HASH_MODULUS);

  if (mpz_sgn(mpz)<0)
      hash = -hash;
  if (~hash == 0)
      hash = -2;

  return hash;
}

size_t fmpq_hash(const fmpq_t q)
{
  /* TODO change to use HASH_MODULUS-computation */
  size_t hq = fmpz_hash( fmpq_denref( q ) );
  return fmpz_hash( fmpq_numref( q ) ) ^ rotate( hq, ROT_BITS );
}

size_t mpfr_hash(mpfr_srcptr f)
{
  size_t hash = 0;
  ssize_t exp;
  size_t msize;
  int sign;

  /* Handle special cases first */
  if (!mpfr_number_p(f))
  {
    if (mpfr_inf_p(f))
      if (mpfr_sgn(f) > 0)
        return HASH_PINF;
      else
        return HASH_MINF;
    else
      return HASH_NAN;
  }

  /* Calculate the number of limbs in the mantissa. */
  msize = (f->_mpfr_prec + mp_bits_per_limb - 1) / mp_bits_per_limb;

  /* Calculate the hash of the mantissa. */
  if (mpfr_sgn(f) > 0)
  {
    hash = mpn_mod_1(f->_mpfr_d, msize, HASH_MODULUS);
    sign = 1;
  }
  else if (mpfr_sgn(f) < 0)
  {
    hash = mpn_mod_1(f->_mpfr_d, msize, HASH_MODULUS);
    sign = -1;
  }
  else
    return 0;

  /* Calculate the final hash. */
  exp = f->_mpfr_exp - (msize * mp_bits_per_limb);
  if( exp >= 0 )
    exp %= HASH_BITS-1;
  else
    exp = HASH_BITS-1-((-1-exp) % (HASH_BITS-1));
  hash = rotate( hash, exp );

  hash *= sign;
  if (~hash == 0)
      hash = -2;
  return hash;
}
