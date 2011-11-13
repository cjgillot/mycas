#include "primepow.hpp"

#include <ulong_extras.h>

#include "util/compare.hpp"

using namespace numeric::detail;

primepow_ring::primepow_ring(const fmpz_t p, long e)
{
  padic_ctx_init( context, p, e, PADIC_TERSE );

  padic_init( unit, context );
  padic_one( unit, context );

  _padic_inv_precompute( preinv, p, e );
}

primepow_ring::~primepow_ring()
{
  _padic_inv_clear( preinv );

  padic_clear( unit, context );

  padic_ctx_clear( context );
}

#define AS( o )  ( *reinterpret_cast<padic_t*>(&o) )

#define op1 AS(a)
#define op2 AS(b)
#define rop AS(r)

// cdtor

void primepow_ring::init(obj_t r, long v) const
{ padic_init( rop, context ); padic_set_si( rop, v, context ); }

void primepow_ring::set (obj_t r, long v) const
{ padic_set_si( rop, v, context ); }

void primepow_ring::copy(obj_t r, const obj_t a) const
{ _padic_set( rop, op1 ); }

void primepow_ring::clear(obj_t r) const
{ padic_clear( rop, context ); }

// unary

void primepow_ring::neg(obj_t r, const obj_t a) const
{ padic_neg( rop, op1, context ); }

void primepow_ring::inv(obj_t r, const obj_t a) const
{
  if(context->N + padic_val(op1) <= 0)
  {
    padic_zero(rop, context);
    return;
  }

  _padic_inv_precomp( padic_unit( rop ), padic_unit( op1 ), const_cast<padic_inv_struct*>(preinv) );
  padic_val( rop ) = - padic_val( op1 );
}

void primepow_ring::incr(obj_t r, const obj_t a) const
{ padic_add( rop, op1, unit, context ); }

void primepow_ring::decr(obj_t r, const obj_t a) const
{ padic_sub( rop, op1, unit, context ); }

// binary

void primepow_ring::add(obj_t r, const obj_t a, const obj_t b) const
{ padic_add( rop, op1, op2, context ); }

void primepow_ring::sub(obj_t r, const obj_t a, const obj_t b) const
{ padic_sub( rop, op1, op2, context ); }

void primepow_ring::mul(obj_t r, const obj_t a, const obj_t b) const
{ padic_mul( rop, op1, op2, context ); }

void primepow_ring::div(obj_t r, const obj_t a, const obj_t b) const
{
  // adapted copy from <flint/padic/div.c>
  padic_t inv_tmp;

  if(_padic_is_zero(op2))
  {
    printf("ERROR (padic_div).  op2 is zero.\n");
    abort();
  }

  if(_padic_is_zero(op1) || (padic_val(op1) - padic_val(op2) >= context->N))
  {
    padic_zero(rop, context);
    return;
  }

  padic_init(inv_tmp, context);

  _padic_inv_precomp( padic_unit( inv_tmp ), padic_unit( op2 ), const_cast<padic_inv_struct*>(preinv) );
  padic_val( inv_tmp ) = - padic_val( op2 );

  padic_mul(rop, op1, inv_tmp, context);

  padic_clear(inv_tmp, context);
}

// ternary

void primepow_ring::addmul(obj_t r, const obj_t a, const obj_t b) const
{
  padic_t tmp;
  padic_init( tmp, context );
  padic_mul( tmp, op1, op2, context );
  padic_add( rop, rop, tmp, context );
  padic_clear( tmp, context );
}

void primepow_ring::submul(obj_t r, const obj_t a, const obj_t b) const
{
  padic_t tmp;
  padic_init( tmp, context );
  padic_mul( tmp, op1, op2, context );
  padic_sub( rop, rop, tmp, context );
  padic_clear( tmp, context );
}

// power

void primepow_ring::pow(obj_t r, const obj_t a, long e) const
{
  fmpz_t pow_tmp;
  int alloc = 0;

  if(_padic_is_zero(op1) || e * padic_val(op1) >= context->N)
  {
    padic_zero(rop, context);
    return;
  }

  if(e > 0)
  {
    /* Form u' = u^e mod p^{N - v'} */
    padic_val(rop) = e * padic_val(op1);

    _padic_ctx_pow_ui(pow_tmp, &alloc, context->N - padic_val(rop), context);
    fmpz_powm_ui(padic_unit(rop), padic_unit(op1), e, pow_tmp);
  }
  else if(e < 0)
  {
    /* Need u^{-1} to precision ceil((N - v e) / -e) */
    _padic_inv_precomp( padic_unit(rop), padic_unit(op1), const_cast<padic_inv_struct*>(preinv) );
    padic_val(rop) = e * padic_val(op1);

    _padic_ctx_pow_ui(pow_tmp, &alloc, context->N - padic_val(rop), context);
    fmpz_powm_ui(padic_unit(rop), padic_unit(rop), -e, pow_tmp);
  }
  else
  {
    _padic_set(rop, unit);
  }

  if(alloc)
    fmpz_clear(pow_tmp);
}

// compare

bool primepow_ring::equal(const obj_t a, const obj_t b) const
{ return padic_equal( op1, op2, context ); }

int  primepow_ring::cmp(const obj_t a, const obj_t b) const
{
  int c = util::compare( padic_val ( op1 ), padic_val ( op2 ) );
  if( c ) return c;
  return       fmpz_cmp( padic_unit( op1 ), padic_unit( op2 ) );
}
