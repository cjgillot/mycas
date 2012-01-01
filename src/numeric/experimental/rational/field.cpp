#include "field.hpp"

#include <fmpq.h>
#include "numeric/mp_operators.hpp"

#include "util/compare.hpp"

using namespace numeric;

#define AS( o )  ( *reinterpret_cast<fmpq_t*>(&o) )

#define op1 AS(a)
#define op2 AS(b)
#define rop AS(r)

// cdtor

void rational_field::init(obj_t r, long n, long d) const
{
  fmpq_init(rop);
  fmpq_set_si( rop, n, d );
}

void rational_field::set (obj_t r, long n, long d) const
{ fmpq_set_si( rop, n, d ); }

#define BIND0( method, function )       \
  rational_field::method( obj_t r ) const \
  { function( rop ); }

#define BIND1( method, function )                      \
  rational_field::method( obj_t r, const obj_t a ) const \
  { function( rop, op1 ); }

#define BIND2( method, function )                                     \
  rational_field::method( obj_t r, const obj_t a, const obj_t b ) const \
  { function( rop, op1, op2 ); }

void BIND1( copy,  fmpq_set )
void BIND0( clear, fmpq_clear )

// unary

void BIND1( neg,   fmpq_neg )

void rational_field::incr(obj_t r) const
{ fmpz_add( fmpq_numref(rop), fmpq_numref(rop), fmpq_denref(rop) ); }

void rational_field::decr(obj_t r) const
{ fmpz_sub( fmpq_numref(rop), fmpq_numref(rop), fmpq_denref(rop) ); }

// binary

void BIND2( add, fmpq_add )
void BIND2( sub, fmpq_sub )
void BIND2( mul, fmpq_mul )
void BIND2( div, fmpq_div )
// void BIND2( mod, fmpq_mod )

// ternary

void BIND2( addmul, fmpq_addmul )
void BIND2( submul, fmpq_submul )

// power

void rational_field::pow(obj_t r, const obj_t a, ulong exp) const
{
  fmpz_pow_ui( fmpq_numref(rop), fmpq_numref(op1), exp );
  fmpz_pow_ui( fmpq_denref(rop), fmpq_denref(op1), exp );
}

void rational_field::pow(obj_t r, const obj_t a, long exp) const
{
  if( exp > 0 )
  {
    fmpz_pow_ui( fmpq_numref(rop), fmpq_numref(op1), exp );
    fmpz_pow_ui( fmpq_denref(rop), fmpq_denref(op1), exp );
  }

  else if( exp < 0 )
  {
    if( rop != op1 )
    { // no alias
      fmpz_pow_ui( fmpq_denref(rop), fmpq_numref(op1), exp );
      fmpz_pow_ui( fmpq_numref(rop), fmpq_denref(op1), exp );
    }
    else
    { // aliased
      fmpz_t den = { 0 };
      fmpz_swap( den, fmpq_denref( rop ) );

      fmpz_pow_ui( fmpq_denref(rop), fmpq_numref(op1), exp );
      fmpz_pow_ui( fmpq_numref(rop), den, exp );

      fmpz_clear( den );
    }
  }

  else
    fmpq_set_si( rop, 1, 1 );
}

// compare

bool rational_field::equal(const obj_t a, const obj_t b) const
{ return fmpq_equal( op1, op2 ); }

int  rational_field::cmp(const obj_t a, const obj_t b) const
{ return fmpq_cmp( op1, op2 ); }
