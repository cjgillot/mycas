#include "number.hpp"

using numeric::number;
using numeric::detail::impl_t;
using numeric::detail::wrapper;

namespace {

#define OP_STRUCT( name, op )                     \
  struct name##_visitor                           \
  : boost::static_visitor<>                       \
  {                                               \
    impl_t* impl;                                 \
                                                  \
    explicit name##_visitor( impl_t &i )          \
    : impl( &i ) {}                               \
                                                  \
    template<class T, class U>                    \
    void operator()(const T &a, const U &b) const \
    { *impl = *a op *b; }                         \
  };

OP_STRUCT( add, + )
OP_STRUCT( sub, - )

}

number number::add(const number &a, const number &b)
{
  number ret;

  boost::apply_visitor( add_visitor( ret.m_impl ), a.m_impl, b.m_impl );

  return ret;
}

number number::sub(const number &a, const number &b)
{
  number ret;

  boost::apply_visitor( sub_visitor( ret.m_impl ), a.m_impl, b.m_impl );

  return ret;
}

namespace {

struct mul_visitor
: boost::static_visitor<>
{
  impl_t* impl;

  explicit mul_visitor( impl_t &i )
  : impl( &i ) {}

  template<class T, class U>
  void operator()(const T &a, const U &b) const
  { *impl = *a * *b; }

  void operator()(const wrapper<fmpq_t> &a, const wrapper<fmpz_t> &b) const
  {
    fmpq_t tmp;
    fmpq_init( tmp );
    fmpq_mul_fmpz( tmp, a->get_fmpq_t(), b->get_fmpz_t() );
    clear_q( tmp );
  }

  void operator()(const wrapper<fmpz_t> &a, const wrapper<fmpq_t> &b) const
  {
    fmpq_t tmp;
    fmpq_init( tmp );
    fmpq_mul_fmpz( tmp, b->get_fmpq_t(), a->get_fmpz_t() );
    clear_q( tmp );
  }

  void operator()(const wrapper<fmpq_t> &a, const wrapper<fmpq_t> &b) const
  {
    fmpq_t tmp;
    fmpq_init( tmp );
    fmpq_mul( tmp, a->get_fmpq_t(), b->get_fmpq_t() );
    clear_q( tmp );
  }

private:
  void clear_q(fmpq_t r) const
  {
    if( fmpz_is_one( fmpq_denref( r ) ) )
    {
      *impl = fmpz_class();
      fmpz* p = boost::get< wrapper<fmpz_t> >( *impl )->get_fmpz_t();
      fmpz_swap( p, fmpq_numref( r ) );
      fmpq_clear( r );
    }
    else
    {
      *impl = fmpq_class();
      fmpq* p = boost::get< wrapper<fmpq_t> >( *impl )->get_fmpq_t();
      fmpq_swap( p, r );
      fmpq_clear( r );
    }
  }
};

}

number number::mul(const number &a, const number &b)
{
  number ret;

  boost::apply_visitor( mul_visitor( ret.m_impl ), a.m_impl, b.m_impl );

  return ret;
}


namespace {

struct div_visitor
: boost::static_visitor<>
{
  impl_t* impl;

  explicit div_visitor( impl_t &i )
  : impl( &i ) {}

  template<class T, class U>
  void operator()(const T &a, const U &b) const
  { *impl = *a / *b; }

  void operator()(const wrapper<fmpz_t> &a, const wrapper<fmpz_t> &b) const
  {
    if( fmpz_is_pm1( b->get_fmpz_t() ) )
      *impl = fmpz_class( *a * *b );
    else
      *impl = fmpq_class( *a, *b );
  }

  void operator()(const wrapper<fmpq_t> &a, const wrapper<fmpq_t> &b) const
  {
    fmpq_t tmp;
    fmpq_init( tmp );
    fmpq_div( tmp, a->get_fmpq_t(), b->get_fmpq_t() );
    clear_q( tmp );
  }

private:
  void clear_q(fmpq_t r) const
  {
    if( fmpz_is_one( fmpq_denref( r ) ) )
    {
      *impl = fmpz_class();
      fmpz* p = boost::get< wrapper<fmpz_t> >( *impl )->get_fmpz_t();
      fmpz_swap( p, fmpq_numref( r ) );
      fmpq_clear( r );
    }
    else
    {
      *impl = fmpq_class();
      fmpq* p = boost::get< wrapper<fmpq_t> >( *impl )->get_fmpq_t();
      fmpq_swap( p, r );
      fmpq_clear( r );
    }
  }
};

}

number number::div(const number &a, const number &b)
{
  number ret;

  boost::apply_visitor( div_visitor( ret.m_impl ), a.m_impl, b.m_impl );

  return ret;
}

#if 0
number number::quo(const number &a, const number &b)
{
  uninitialized_t ut;
  number ret ( ut );
  fmpz_init( ret.m_impl.z );

  switch( COMBINE( a.m_tag, b.m_tag ) )
  {
  case ii_tag:
    fmpz_mod( ret.m_impl.z, a.m_impl.z, b.m_impl.z );
    break;

  case qi_tag:
    fmpq_init( ret.m_impl.q );
    fmpq_div_fmpz( ret.m_impl.q, a.m_impl.q, b.m_impl.z );
    goto clear_q;

  case iq_tag:
    fmpq_init( ret.m_impl.q );
    fmpq_div_fmpz( ret.m_impl.q, b.m_impl.q, a.m_impl.z );
    fmpz_swap( fmpq_numref( ret.m_impl.q ), fmpq_denref( ret.m_impl.q ) );
    goto clear_q;

  case qq_tag:
    fmpq_init( ret.m_impl.q );
    fmpq_mul( ret.m_impl.q, a.m_impl.q, b.m_impl.q );

  clear_q:
    ret.m_tag = rat_tag;
    if( fmpz_is_one( fmpq_denref( ret.m_impl.q ) ) )
    {
      ASSERT( ret.m_impl.z == fmpq_numref( ret.m_impl.q ) );
      fmpz_clear( fmpq_denref( ret.m_impl.q ) );
      ret.m_tag = int_tag;
    }
    break;

  case qf_tag:
    mpfr_init( ret.m_impl.f );
    fmpq_get_mpfr( ret.m_impl.f, a.m_impl.q, GMP_RNDN );
    mpfr_div( ret.m_impl.f, ret.m_impl.f, b.m_impl.f, GMP_RNDN );
    ret.m_tag = flt_tag;
    break;

  case fq_tag:
    mpfr_init( ret.m_impl.f );
    fmpq_get_mpfr( ret.m_impl.f, b.m_impl.q, GMP_RNDN );
    mpfr_div( ret.m_impl.f, a.m_impl.f, ret.m_impl.f, GMP_RNDN );
    ret.m_tag = flt_tag;
    break;

  case ff_tag:
    mpfr_init( ret.m_impl.f );
    mpfr_div( ret.m_impl.f, a.m_impl.f, b.m_impl.f, GMP_RNDN );
    ret.m_tag = flt_tag;
    break;

  case fi_tag:
    mpfr_init( ret.m_impl.f );
    if( COEFF_IS_MPZ( *b.m_impl.z ) )
      mpfr_div_z ( ret.m_impl.f, a.m_impl.f, COEFF_TO_PTR( *b.m_impl.z ), GMP_RNDN );
    else
      mpfr_div_si( ret.m_impl.f, a.m_impl.f, *b.m_impl.z, GMP_RNDN );
    ret.m_tag = flt_tag;
    break;

  case if_tag:
    mpfr_init( ret.m_impl.f );
    mpfr_set_fz( ret.m_impl.f, a.m_impl.z, GMP_RNDN );
    mpfr_div( ret.m_impl.f, ret.m_impl.f, b.m_impl.f, GMP_RNDN );
    ret.m_tag = flt_tag;
    break;

  default: ASSERT( false ); return 0;
  }

  return ret;
}

number number::mod(const number &a, const number &b)
{
  uninitialized_t ut;
  number ret ( ut );

  switch( COMBINE( a.m_tag, b.m_tag ) )
  {
  case ii_tag:
    fmpq_init( ret.m_impl.q );
    fmpq_set_fmpz_frac( ret.m_impl.q, a.m_impl.z, b.m_impl.z );
    goto clear_q;

  case qi_tag:
    fmpq_init( ret.m_impl.q );
    fmpq_div_fmpz( ret.m_impl.q, a.m_impl.q, b.m_impl.z );
    goto clear_q;

  case iq_tag:
    fmpq_init( ret.m_impl.q );
    fmpq_div_fmpz( ret.m_impl.q, b.m_impl.q, a.m_impl.z );
    fmpz_swap( fmpq_numref( ret.m_impl.q ), fmpq_denref( ret.m_impl.q ) );
    goto clear_q;

  case qq_tag:
    fmpq_init( ret.m_impl.q );
    fmpq_mul( ret.m_impl.q, a.m_impl.q, b.m_impl.q );

  clear_q:
    ret.m_tag = rat_tag;
    if( fmpz_is_one( fmpq_denref( ret.m_impl.q ) ) )
    {
      ASSERT( ret.m_impl.z == fmpq_numref( ret.m_impl.q ) );
      fmpz_clear( fmpq_denref( ret.m_impl.q ) );
      ret.m_tag = int_tag;
    }
    break;

  case qf_tag:
    mpfr_init( ret.m_impl.f );
    fmpq_get_mpfr( ret.m_impl.f, a.m_impl.q, GMP_RNDN );
    mpfr_div( ret.m_impl.f, ret.m_impl.f, b.m_impl.f, GMP_RNDN );
    ret.m_tag = flt_tag;
    break;

  case fq_tag:
    mpfr_init( ret.m_impl.f );
    fmpq_get_mpfr( ret.m_impl.f, b.m_impl.q, GMP_RNDN );
    mpfr_div( ret.m_impl.f, a.m_impl.f, ret.m_impl.f, GMP_RNDN );
    ret.m_tag = flt_tag;
    break;

  case ff_tag:
    mpfr_init( ret.m_impl.f );
    mpfr_div( ret.m_impl.f, a.m_impl.f, b.m_impl.f, GMP_RNDN );
    ret.m_tag = flt_tag;
    break;

  case fi_tag:
    mpfr_init( ret.m_impl.f );
    if( COEFF_IS_MPZ( *b.m_impl.z ) )
      mpfr_div_z ( ret.m_impl.f, a.m_impl.f, COEFF_TO_PTR( *b.m_impl.z ), GMP_RNDN );
    else
      mpfr_div_si( ret.m_impl.f, a.m_impl.f, *b.m_impl.z, GMP_RNDN );
    ret.m_tag = flt_tag;
    break;

  case if_tag:
    mpfr_init( ret.m_impl.f );
    mpfr_set_fz( ret.m_impl.f, a.m_impl.z, GMP_RNDN );
    mpfr_div( ret.m_impl.f, ret.m_impl.f, b.m_impl.f, GMP_RNDN );
    ret.m_tag = flt_tag;
    break;

  default: ASSERT( false ); return 0;
  }

  return ret;
}

#endif
