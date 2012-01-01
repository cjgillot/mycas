#include "primepow.hpp"

#include <ulong_extras.h>
#include <padic.h>

#include "util/compare.hpp"

using namespace numeric::detail_modular;

primepow_ring::primepow_ring(const fmpz_t p, ulong e)
: basecase_ring(p), prime(*modulus), exp(e)
{
  *modulus = 0;
  fmpz_pow_ui(modulus, p, e);

  if( e > 1 )
    _padic_inv_precompute( preinv, p, e );
}

primepow_ring::~primepow_ring()
{
  fmpz_clear(&prime);
}

#define AS( o )  ( *reinterpret_cast<fmpz_t*>(&o) )

#define op1 AS(a)
#define op2 AS(b)
#define rop AS(r)

bool primepow_ring::inv(obj_t r, const obj_t a) const
{
  if( exp == 1 )
    return fmpz_invmod( rop, op1, modulus );

  else
    _padic_inv_precomp( rop, op1, const_cast<padic_inv_struct*>(preinv) );
}

bool primepow_ring::div(obj_t r, const obj_t a, const obj_t b) const
{
  fmpz_t binv;

  fmpz_init(binv);

  if( exp == 1 )
  {
    if( ! fmpz_invmod( binv, op2, modulus ) )
    {
      fmpz_clear( binv );
      return false;
    }
  }

  else
    _padic_inv_precomp( binv, op2, const_cast<padic_inv_struct*>(preinv) );

  fmpz_mul( rop, op1, binv );
  fmpz_mod( rop, rop, modulus );

  fmpz_clear( binv );

  return true;
}

bool primepow_ring::pow(obj_t r, const obj_t a, long e) const
{
  if(e > 0)
    fmpz_powm_ui( rop, op1, exp, modulus );

  else if(e < 0)
  {
    if( exp == 1 )
      fmpz_invmod( rop, op1, modulus );

    else
      _padic_inv_precomp( rop, op1, const_cast<padic_inv_struct*>(preinv) );

    fmpz_powm_ui( rop, rop, -exp, modulus );
  }

  else
    fmpz_set( rop, op1 );
}
