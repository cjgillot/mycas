#include "flint.hpp"

#define HASH_BITS (8 * sizeof(size_t))
#define ROT_BITS ((HASH_BITS / 5) + 2) // pure random choice

#define HASH_MODULUS (~((size_t)0))
#define HASH_INF  314159
#define HASH_NAN  0

#define rotate( xx, bits )  \
  ( ( xx << bits ) | ( xx >> ( HASH_BITS - bits ) ) )

static inline size_t
fmpz_hash(const fmpz_t z)
{
  if( !COEFF_IS_MPZ( *z ) )
    return *z;

  __mpz_struct* mpz = COEFF_TO_PTR( *z );

  size_t hash = mpn_mod_1(mpz->_mp_d, mpz_size(mpz), HASH_MODULUS);

  if (mpz_sgn(mpz)<0)
      hash = -hash;
  if (~hash == 0)
      hash = -2;

  return hash;
}

static inline size_t
fmpq_hash(const fmpq_t q)
{
  // TODO change to use HASH_MODULUS-computation
  size_t hq = fmpz_hash( fmpq_denref( q ) );
  return fmpz_hash( fmpq_numref( q ) ) ^ rotate( hq, ROT_BITS );
}

static inline size_t
mpfr_hash(mpfr_srcptr f)
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
        return HASH_INF;
      else
        return -HASH_INF;
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

#include "number.hpp"

using numeric::number;
using numeric::detail::wrapper;

namespace {

struct hash_visitor
: public boost::static_visitor<size_t>
{
  size_t operator()(const wrapper<fmpz_t> &c) const
  { return fmpz_hash( c->get_fmpz_t() ); }
  size_t operator()(const wrapper<fmpq_t> &c) const
  { return fmpq_hash( c->get_fmpq_t() ); }
  size_t operator()(const wrapper<mpfr_t> &c) const
  { return mpfr_hash( c->get_mpfr_t() ); }
};

}

size_t number::hash() const
{ return boost::apply_visitor( hash_visitor(), m_impl ); }

namespace {

struct cmp_visitor
: public boost::static_visitor<util::cmp_t>
{
  util::cmp_t operator()(const wrapper<fmpz_t> &a, const wrapper<fmpz_t> &b) const
  { return fmpz_cmp( a->get_fmpz_t(), b->get_fmpz_t() ); }

  util::cmp_t operator()(const wrapper<fmpq_t> &a, const wrapper<fmpq_t> &b) const
  { return fmpq_cmp( a->get_fmpq_t(), b->get_fmpq_t() ); }

  util::cmp_t operator()(const wrapper<mpfr_t> &a, const wrapper<mpfr_t> &b) const
  { return mpfr_cmp( a->get_mpfr_t(), b->get_mpfr_t() ); }

  util::cmp_t operator()(const wrapper<fmpz_t> &a, const wrapper<fmpq_t> &b) const
  {
    fmpq tmp = { *a->get_fmpz_t(), 1 };
    return fmpq_cmp( &tmp, b->get_fmpq_t() );
  }
  util::cmp_t operator()(const wrapper<fmpq_t> &a, const wrapper<fmpz_t> &b) const
  {
    fmpq tmp = { *b->get_fmpz_t(), 1 };
    return fmpq_cmp( a->get_fmpq_t(), &tmp );
  }

  util::cmp_t operator()(const wrapper<fmpq_t> &a, const wrapper<mpfr_t> &b) const
  {
    mpfr_t tmp;
    mpfr_init( tmp ); fmpq_get_mpfr( tmp, a->get_fmpq_t(), GMP_RNDN );
    util::cmp_t ret = mpfr_cmp( tmp, b->get_mpfr_t() );
    mpfr_clear( tmp );
    return ret;
  }
  util::cmp_t operator()(const wrapper<mpfr_t> &a, const wrapper<fmpq_t> &b) const
  {
    mpfr_t tmp;
    mpfr_init( tmp ); fmpq_get_mpfr( tmp, b->get_fmpq_t(), GMP_RNDN );
    util::cmp_t ret = mpfr_cmp( a->get_mpfr_t(), tmp );
    mpfr_clear( tmp );
    return ret;
  }

  util::cmp_t operator()(const wrapper<fmpz_t> &a, const wrapper<mpfr_t> &b) const
  {
    mpfr_t tmp;
    mpfr_init( tmp ); mpfr_set_fz( tmp, a->get_fmpz_t(), GMP_RNDN );
    util::cmp_t ret = mpfr_cmp( tmp, b->get_mpfr_t() );
    mpfr_clear( tmp );
    return ret;
  }
  util::cmp_t operator()(const wrapper<mpfr_t> &a, const wrapper<fmpz_t> &b) const
  {
    mpfr_t tmp;
    mpfr_init( tmp ); mpfr_set_fz( tmp, b->get_fmpz_t(), GMP_RNDN );
    util::cmp_t ret = mpfr_cmp( a->get_mpfr_t(), tmp );
    mpfr_clear( tmp );
    return ret;
  }
};

}

util::cmp_t number::
compare(const number &a, const number &b)
{ return boost::apply_visitor( cmp_visitor(), a.m_impl, b.m_impl ); }
