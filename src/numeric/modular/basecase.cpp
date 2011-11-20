#include "basecase.hpp"

#include <ulong_extras.h>

#include "util/compare.hpp"

using namespace numeric::detail_modular;

basecase_ring::basecase_ring(const fmpz_t m)
{
  fmpz_init(modulus);
  fmpz_set(modulus, m);
}

basecase_ring::~basecase_ring()
{
  fmpz_clear(modulus);
}

#define AS( o )  ( *reinterpret_cast<fmpz_t*>(&o) )

#define op1 AS(a)
#define op2 AS(b)
#define rop AS(r)

// cdtor

void basecase_ring::init(obj_t r, long v) const
{
  fmpz_init(rop);
  fmpz_set_si(rop, v);
  fmpz_mod(rop, rop, modulus);
}

void basecase_ring::set (obj_t r, long v) const
{
  fmpz_set_si( rop, v );
  fmpz_mod(rop, rop, modulus);
}

void basecase_ring::copy(obj_t r, const obj_t a) const
{ fmpz_set( rop, op1 ); }

void basecase_ring::clear(obj_t r) const
{ fmpz_clear( rop ); }

// unary

void basecase_ring::neg(obj_t r, const obj_t a) const
{
  if( fmpz_is_zero(op1) )
    fmpz_set(rop, op1);
  else
    fmpz_sub(rop, modulus, op1);
}

bool basecase_ring::inv(obj_t r, const obj_t a) const
{ return fmpz_invmod(rop, op1, modulus); }

void basecase_ring::incr(obj_t r, const obj_t a) const
{
  fmpz_add_ui( rop, op1, 1 );
  if( fmpz_equal(rop, op1) )
    fmpz_set_ui(rop, 0);
}

void basecase_ring::decr(obj_t r, const obj_t a) const
{
  if( fmpz_is_zero(rop) )
    fmpz_set(rop, modulus);
  fmpz_sub_ui( rop, op1, 1 );
}

// binary

void basecase_ring::add(obj_t r, const obj_t a, const obj_t b) const
{
  fmpz_add( rop, op1, op2 );
  if( fmpz_cmp(rop, op1) >= 0 )
    fmpz_sub(rop, rop, modulus);
}

void basecase_ring::sub(obj_t r, const obj_t a, const obj_t b) const
{
  fmpz_sub( rop, op1, op2 );
  if( fmpz_sgn(rop) < 0 )
    fmpz_add(rop, rop, modulus);
}

void basecase_ring::mul(obj_t r, const obj_t a, const obj_t b) const
{
  fmpz_mul( rop, op1, op2 );
  fmpz_mod( rop, rop, modulus );
}

bool basecase_ring::div(obj_t r, const obj_t a, const obj_t b) const
{
  fmpz_t binv;
  fmpz_init(binv);

  if( ! fmpz_invmod(binv, op2, modulus) )
    return false;

  fmpz_mul( rop, op1, op2 );
  fmpz_mod( rop, rop, modulus );

  fmpz_clear(binv);

  return true;
}

// ternary

void basecase_ring::addmul(obj_t r, const obj_t a, const obj_t b) const
{
  fmpz_addmul( rop, op1, op2 );
  fmpz_mod( rop, rop, modulus );
}

void basecase_ring::submul(obj_t r, const obj_t a, const obj_t b) const
{
  fmpz_submul( rop, op1, op2 );
  fmpz_mod( rop, rop, modulus );
}

// power

bool basecase_ring::pow(obj_t r, const obj_t a, long exp) const
{
  if(exp > 0)
    fmpz_powm_ui( rop, op1, exp, modulus );

  else if(exp < 0)
  {
    if( ! fmpz_invmod( rop, op1, modulus ) )
      return false;

    fmpz_powm_ui( rop, rop, -exp, modulus );
  }

  else
    fmpz_set( rop, op1 );

  return true;
}

// compare

bool basecase_ring::equal(const obj_t a, const obj_t b) const
{ return fmpz_equal( op1, op2 ); }

int  basecase_ring::cmp(const obj_t a, const obj_t b) const
{ return fmpz_cmp( op1, op2 ); }
