#include "numeric/real/real_t.hpp"

#include "pow.hpp"

namespace numeric {

namespace intpow {

// alias-safe
static inline void
pow_z_p(mpz_ptr r, mpz_srcptr b, mpz_srcptr e)
{
  assert( mpz_sgn( e ) > 0 );
  assert( mpz_fits_ulong_p( e ) );

  mpz_pow_ui( r, b, mpz_get_ui(e) );
}

// alias-safe
static inline void
pow_z_m(mpq_ptr r, mpz_srcptr b, mpz_srcptr e)
{
  assert( mpz_sgn( e ) < 0 );

  mpz_t me; mpz_init( me );
  mpz_neg( me, e );

  // ensure safety if b is r's numerator
  pow_z_p( mpq_denref( r ), b, me );
  mpz_set_ui( mpq_numref( r ), 1 );

  mpz_clear( me );
}

// alias-safe
static inline void
pow_q_p(mpq_ptr r, mpq_srcptr b, mpz_srcptr e)
{
  assert( mpz_sgn( e ) > 0 );
  assert( mpz_fits_ulong_p( e ) );

  pow_z_p( mpq_numref( r ), mpq_numref( b ), e );
  pow_z_p( mpq_denref( r ), mpq_denref( b ), e );
}

// _not_ alias-safe !!!
static inline void
pow_q_m(mpq_ptr r, mpq_srcptr b, mpz_srcptr e)
{
  assert( mpz_sgn( e ) < 0 );
  assert( mpz_fits_ulong_p( e ) );

  mpz_t me; mpz_init( me );
  mpz_neg( me, e );

  pow_z_p( mpq_numref( r ), mpq_denref( b ), me );
  pow_z_p( mpq_denref( r ), mpq_numref( b ), me );

  mpz_clear( me );
}

}

template<class T, class U>
static inline real_t*
floatpow(const T &b, const U &e)
{
  mpfr_class r = pow(b,e);
  if( mpfr_nan_p( r.get_mpfr_t() ) )
    return 0;
  return new real_t( r );
}

namespace ratpow {

// alias-safe
static inline bool
root_z(mpz_ptr r, mpz_srcptr b, mpz_srcptr ed)
{
  assert( mpz_sgn( ed ) > 0 );
  assert( mpz_fits_ulong_p( ed ) );

  return mpz_root( r, b, mpz_get_ui( ed ) );
}

// alias-safe
static inline bool
pow_z_p0(mpz_ptr r, mpz_srcptr b, mpq_srcptr e)
{
  assert( mpz_fits_ulong_p( mpq_numref( e ) ) );
  assert( mpz_fits_ulong_p( mpq_denref( e ) ) );

  // a GMP denominator is always > 0
  if( ! root_z( r, b, mpq_denref( e ) ) )
    return false;

  intpow::pow_z_p( r, r, mpq_numref( e ) );

  return true;
}

// alias-safe
static inline bool
pow_z_p(mpz_ptr r, mpz_srcptr b, mpq_srcptr e)
{
  if( ! mpz_fits_ulong_p( mpq_numref( e ) )
   || ! mpz_fits_ulong_p( mpq_denref( e ) ) )
    return false;

  return pow_z_p0( r, b, e );
}

// alias-safe
static inline bool
pow_z_m(mpq_ptr r, mpz_srcptr b, mpq_srcptr e)
{
  if( ! mpz_fits_ulong_p( mpq_numref( e ) )
   || ! mpz_fits_ulong_p( mpq_denref( e ) ) )
    return false;

  // a GMP denominator is always > 0
  if( ! root_z( mpq_numref( r ), b, mpq_denref( e ) ) )
    return false;

  intpow::pow_z_m( r, mpq_numref( r ), mpq_numref( e ) );

  return true;
}

// alias-safe
static inline bool
pow_q_p(mpq_ptr r, mpq_srcptr b, mpq_srcptr e)
{
  if( ! mpz_fits_ulong_p( mpq_numref( e ) )
   || ! mpz_fits_ulong_p( mpq_denref( e ) ) )
    return false;

  // TODO: order short-circuit according to the sizes
  return pow_z_p0( mpq_numref( r ), mpq_numref( b ), e )
      && pow_z_p0( mpq_denref( r ), mpq_denref( b ), e );
}

// _not_ alias-safe !!!
static inline bool
pow_q_m(mpq_ptr r, mpq_srcptr b, mpq_srcptr e)
{
  if( ! mpz_fits_ulong_p( mpq_numref( e ) )
   || ! mpz_fits_ulong_p( mpq_denref( e ) ) )
    return false;

  // TODO: order short-circuit according to the sizes
  return pow_z_p0( mpq_numref( r ), mpq_denref( b ), e )
      && pow_z_p0( mpq_denref( r ), mpq_numref( b ), e );
}

}

namespace {

struct power_v
: boost::static_visitor<real_t*> {

  typedef real_t ret_t;

  // floating exponent case (the easy one)
  template<class T>
  inline ret_t*
  operator()(const T &b, const mpfr_class &e) const
  { return floatpow(b, e); }

  template<class U>
  inline ret_t*
  operator()(const mpfr_class &b, const U &e) const
  { return floatpow(b, e); }

  inline ret_t*
  operator()(const mpfr_class &b, const mpfr_class &e) const
  { return floatpow(b, e); }

  // integer exponent case
  inline ret_t*
  operator()(const mpz_class &b, const mpz_class &e) const
  {
    const int s = sgn( e );

    if( ! e.fits_ulong_p() )
      return 0;

    if( s <  0 )
    {
      mpq_class r;
      intpow::pow_z_m( r.get_mpq_t(), b.get_mpz_t(), e.get_mpz_t() );
      return new real_t( r );
    }

    //( s >  0 )
    {
      mpz_class r;
      intpow::pow_z_p( r.get_mpz_t(), b.get_mpz_t(), e.get_mpz_t() );
      return new real_t( r );
    }
  }

  inline ret_t*
  operator()(const mpq_class &b, const mpz_class &e) const
  {
    const int s = sgn( e );

    if( ! e.fits_ulong_p() )
      return 0;

    if( s <  0 )
    {
      if( b.get_num() == 1 ) {
        mpz_class r, me = - e;
        intpow::pow_z_p( r.get_mpz_t(), b.get_den_mpz_t(), me.get_mpz_t() );
        return new real_t( r );
      }

      mpq_class r;
      intpow::pow_q_m( r.get_mpq_t(), b.get_mpq_t(), e.get_mpz_t() );
      return new real_t( r );
    }

    //( s >  0 )
    {
      if( b.get_den() == 1 ) {
        mpz_class r;
        intpow::pow_z_p( r.get_mpz_t(), b.get_num_mpz_t(), e.get_mpz_t() );
        return new real_t( r );
      }

      mpq_class r;
      intpow::pow_q_p( r.get_mpq_t(), b.get_mpq_t(), e.get_mpz_t() );
      return new real_t( r );
    }
  }

  // rational exponent case (the hard one)
  inline ret_t*
  operator()(const mpz_class &b, const mpq_class &e) const
  {
    if( e.get_den() == 1 )
      return operator()( b, e.get_num() );

    const int s = sgn( e );

    if( s <  0 )
    {
      mpq_class r;
      if( ! ratpow::pow_z_m( r.get_mpq_t(), b.get_mpz_t(), e.get_mpq_t() ) )
        return 0;
      return new real_t( r );
    }

    //( s >  0 )
    {
      mpz_class r;
      if( ! ratpow::pow_z_p( r.get_mpz_t(), b.get_mpz_t(), e.get_mpq_t() ) )
        return 0;
      return new real_t( r );
    }
  }

  inline ret_t*
  operator()(const mpq_class &b, const mpq_class &e) const
  {
    if( e.get_den() == 1 )
      return operator()( b, e.get_num() );

    const int s = sgn( e );

    if( s <  0 )
    {
      if( b.get_num() == 1 ) {
        mpz_class r;
        mpq_class me = - e;

        if(! ratpow::pow_z_p( r.get_mpz_t(), b.get_den_mpz_t(), me.get_mpq_t() ) )
          return 0;

        return new real_t( r );
      }

      mpq_class r;
      if( ! ratpow::pow_q_m( r.get_mpq_t(), b.get_mpq_t(), e.get_mpq_t() ) )
        return 0;
      return new real_t( r );
    }

    //( s >  0 )
    {
      if( b.get_den() == 1 ) {
        mpz_class r;
        if( ! ratpow::pow_z_p( r.get_mpz_t(), b.get_num_mpz_t(), e.get_mpq_t() ) )
          return 0;
        return new real_t( r );
      }

      mpq_class r;
      if( ! ratpow::pow_q_p( r.get_mpq_t(), b.get_mpq_t(), e.get_mpq_t() ) )
        return 0;
      return new real_t( r );
    }
  }
};

}

inline const real_t*
real_t::pow(const real_t &b, const real_t &e)
{
  if( e.unit() ) return &b;
  if( b.null() ) return real_t::zero();
  if( e.null() || b.unit() ) return real_t::one();

  return boost::apply_visitor( power_v(), b.m_impl, e.m_impl );
}


// integer exponent case
mpq_class pow(const mpz_class &b, const mpz_class &e) throw(bad_pow)
{
  const int s = sgn( e );

  if( s == 0 )
    return 1;

  if( ! e.fits_ulong_p() )
    throw bad_pow();

  mpq_class ret;

  if( s <  0 )
    intpow::pow_z_m( ret.get_mpq_t(), b.get_mpz_t(), e.get_mpz_t() );

  else //( s >  0 )
    intpow::pow_z_p( ret.get_num_mpz_t(), b.get_mpz_t(), e.get_mpz_t() );

  return ret;
}

mpq_class pow(const mpq_class &b, const mpz_class &e) throw(bad_pow)
{
  const int s = sgn( e );

  if( s == 0 )
    return 1;

  if( ! e.fits_ulong_p() )
    throw bad_pow();

  mpq_class ret;

  if( s <  0 )
    intpow::pow_q_m( ret.get_mpq_t(), b.get_mpq_t(), e.get_mpz_t() );

  else//( s >  0 )
    intpow::pow_q_p( ret.get_mpq_t(), b.get_mpq_t(), e.get_mpz_t() );

  return ret;
}

// rational exponent case (the hard one)
mpq_class pow(const mpz_class &b, const mpq_class &e) throw(bad_pow)
{
  const int s = sgn( e );

  if( s == 0 )
    return 1;

  if( e.get_den() == 1 )
    return pow( b, e.get_num() );

  mpq_class ret;

  if( s <  0 )  {
    if( ! ratpow::pow_z_m( ret.get_mpq_t(), b.get_mpz_t(), e.get_mpq_t() ) )
      throw bad_pow();
  }

  else { //( s >  0 )
    if( ! ratpow::pow_z_p( ret.get_num_mpz_t(), b.get_mpz_t(), e.get_mpq_t() ) )
      throw bad_pow();
  }

  return ret;
}

mpq_class pow(const mpq_class &b, const mpq_class &e) throw(bad_pow)
{
  if( e.get_den() == 1 )
    return pow( b, e.get_num() );

  const int s = sgn( e );

  mpq_class ret;

  if( s <  0 ) {
    if( ! ratpow::pow_q_m( ret.get_mpq_t(), b.get_mpq_t(), e.get_mpq_t() ) )
      throw bad_pow();
  }

  else { //( s >  0 )
    if( ! ratpow::pow_q_p( ret.get_mpq_t(), b.get_mpq_t(), e.get_mpq_t() ) )
      throw bad_pow();
  }

  return ret;
}

mpfr_class pow(const mpfr_class &b, const mpq_class &e) throw(bad_pow)
{
  if( e.get_den() == 1 )
    return pow( b, e.get_num() );

  if( ! e.get_den().fits_ulong_p() )
    return pow(b, mpfr_class(e));

  return pow( root( b, e.get_den().get_ui() ), e.get_num() );
}

} // namespace numeric
