#include "mp_operators.hpp"

#include <iostream>
#include <string>

mpfr_rnd_t  MpFrC::rnd = GMP_RNDN;
int         MpFrC::base = 10;

// from mpirxx
extern std::istream &__gmpz_operator_in_nowhite(std::istream &i, mpz_ptr z, char c);

std::istream& fmpfr_parse(std::istream &is, fmpz_t zz)
{
  mpz_ptr zp = _fmpz_promote( zz );

  if( is >> zp )
    _fmpz_demote_val( zz );

  return is;
}

std::istream& fmpfr_parse(std::istream &is, fmpq_t qq)
{
  if( ! fmpfr_parse( is, fmpq_numref( qq ) ) )
    return is;

  char  c = 0;
  is.get(c); // start reading

  if (c == '/')
  {
    // skip slash, read denominator
    is.get(c);

    mpz_ptr dp = _fmpz_promote( fmpq_denref( qq ) );
    if( __gmpz_operator_in_nowhite(is, dp, c) )
      _fmpz_demote_val( fmpq_denref( qq ) );
  }
  else
  {
    fmpz_one( fmpq_denref( qq ) );

    if (is.good())
      is.putback(c);
    else if (is.eof())
      is.clear();
  }

  return is;
}

std::istream& fmpfr_parse(std::istream &is, mpfr_ptr ff)
{
  std::string tmp;
  is >> tmp;
  mpfr_set_str(
    ff, tmp.c_str(),
    mpfr_class::get_base(),
    mpfr_class::get_rnd()
  );
  return is;
}

std::ostream& fmpfr_print(std::ostream &os, const fmpz_t zz)
{
  if( COEFF_IS_MPZ( *zz ) )
    os << COEFF_TO_PTR( *zz );
  else
    os << *zz;
  return os;
}

std::ostream& fmpfr_print(std::ostream &os, const fmpq_t qq)
{
  if( fmpq_is_zero( qq ) )
    os << 0;
  else
  {
    fmpfr_print( os, fmpq_numref( qq ) );
    if( ! fmpz_is_one( fmpq_denref( qq ) ) )
      fmpfr_print( os << '/', fmpq_denref( qq ) );
  }
  return os;
}

std::ostream& fmpfr_print(std::ostream &os, mpfr_srcptr a)
{
  char *s, *t, *s0;
  mp_exp_t  e;

  // for debugging:
  // mpfr_out_str(stdout, 10, 0, a, RND); printf("\n");

  if (mpfr_nan_p(a)) {
    os << "@NaN@";
    return os;
  }

  if (mpfr_inf_p(a)) {
    if (MPFR_SIGN(a) > 0)
      os << "@Inf@";
    else
      os << "-@Inf@";
    return os;
  }

  if (mpfr_zero_p(a)) {
    if (MPFR_SIGN(a) > 0)
      os << "0";
    else
      os << "-0";
    return os;
  }

  s = mpfr_get_str (NULL, &e,
        mpfr_class::get_base(),
        0, a,
        mpfr_class::get_rnd());

  t = mpfr_get_str (NULL, &e,
        mpfr_class::get_base(),
        os.precision(), a,
        mpfr_class::get_rnd());

  if (strlen(s)<strlen(t))
    mpfr_free_str(t);
  else {
    mpfr_free_str(s);
    s = t;
  }

  s0 = s;
  /* for a=3.1416 we have s = "31416" and e = 1 */

  if (*s == '-')
    os.put(*s++);

  /* outputs mantissa */
  os.put(*s++); e--; /* leading digit */
  os.put('.');
  while (*s != '\0')
    os.put(*s++);       /* rest of mantissa */

  mpfr_free_str(s0);

  /* outputs exponent */
  if (e)
    os << (mpfr_class::get_base() <= 10 ? 'e' : '@') << (long) e;

  return os;
}
