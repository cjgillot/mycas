#include "numeric/mp_operators.hpp"

#include "field.hpp"

#include <mpfr.h>

#include "util/compare.hpp"

using namespace numeric;

#define AS( o )  ( *reinterpret_cast<mpfr_t*>(&o) )

#define op1 AS(a)
#define op2 AS(b)
#define rop AS(r)

// cdtor

void real_field::init(obj_t r, long n, long d) const
{
  mpfr_init_set_si( rop, n, rnd );
  mpfr_div_si( rop, rop, d, rnd );
}

void real_field::set (obj_t r, long n, long d) const
{
  mpfr_set_si( rop, n, rnd );
  mpfr_div_si( rop, rop, d, rnd );
}

#define BIND0( method, function )       \
  real_field::method( obj_t r ) const \
  { function( rop, rnd ); }

#define BIND1( method, function )                      \
  real_field::method( obj_t r, const obj_t a ) const \
  { function( rop, op1, rnd ); }

#define BIND2( method, function )                                     \
  real_field::method( obj_t r, const obj_t a, const obj_t b ) const \
  { function( rop, op1, op2, rnd ); }

void BIND1( copy,  mpfr_set )
void real_field::clear( obj_t r ) const
{ mpfr_clear( rop ); }

// unary

void BIND1( neg,   mpfr_neg )

void real_field::incr(obj_t r) const
{ mpfr_add_ui( rop, rop, 1, rnd ); }

void real_field::decr(obj_t r) const
{ mpfr_sub_ui( rop, rop, 1, rnd ); }

// binary

void BIND2( add, mpfr_add )
void BIND2( sub, mpfr_sub )
void BIND2( mul, mpfr_mul )
void BIND2( div, mpfr_div )
// void BIND2( mod, mpfr_mod )

// ternary

void real_field::addmul( obj_t r, const obj_t a, const obj_t b ) const
{
  mpfr_t tmp;
  mpfr_init( tmp );
  mpfr_mul( tmp, op1, op2, rnd );
  mpfr_add( rop, rop, tmp, rnd );
  mpfr_clear( tmp );
}
void real_field::submul( obj_t r, const obj_t a, const obj_t b ) const
{
  mpfr_t tmp;
  mpfr_init( tmp );
  mpfr_mul( tmp, op1, op2, rnd );
  mpfr_sub( rop, rop, tmp, rnd );
  mpfr_clear( tmp );
}

// power

void real_field::pow(obj_t r, const obj_t a, ulong exp) const
{ mpfr_pow_ui( rop, op1, exp, rnd ); }

void real_field::pow(obj_t r, const obj_t a, long exp) const
{ mpfr_pow_si( rop, op1, exp, rnd ); }

// compare

bool real_field::equal(const obj_t a, const obj_t b) const
{ return mpfr_cmp( op1, op2 ) == 0; }

int  real_field::cmp(const obj_t a, const obj_t b) const
{ return mpfr_cmp( op1, op2 ); }
