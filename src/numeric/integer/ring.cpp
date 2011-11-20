#include "ring.hpp"

#include <fmpz.h>

#include "util/compare.hpp"

using namespace numeric;

#define AS( o )  ( *reinterpret_cast<fmpz_t*>(&o) )

#define op1 AS(a)
#define op2 AS(b)
#define rop AS(r)

// cdtor

void integer_ring::init(obj_t r, long v) const
{
  fmpz_init(rop);
  fmpz_set_si(rop, v);
}

void integer_ring::set (obj_t r, long v) const
{ fmpz_set_si( rop, v ); }

#define BIND0( method, function )       \
  integer_ring::method( obj_t r ) const \
  { function( rop ); }

#define BIND1( method, function )                      \
  integer_ring::method( obj_t r, const obj_t a ) const \
  { function( rop, op1 ); }

#define BIND2( method, function )                                     \
  integer_ring::method( obj_t r, const obj_t a, const obj_t b ) const \
  { function( rop, op1, op2 ); }

void BIND1( copy,  fmpz_set )
void BIND0( clear, fmpz_clear )

// unary

void BIND1( neg,   fmpz_neg )

void integer_ring::incr(obj_t r, const obj_t a) const
{ fmpz_add_ui( rop, op1, 1 ); }

void integer_ring::decr(obj_t r, const obj_t a) const
{ fmpz_sub_ui( rop, op1, 1 ); }

// binary

void BIND2( add, fmpz_add )
void BIND2( sub, fmpz_sub )
void BIND2( mul, fmpz_mul )
void BIND2( div, fmpz_fdiv_q )
void BIND2( mod, fmpz_mod )

// ternary

void BIND2( addmul, fmpz_addmul )
void BIND2( submul, fmpz_submul )

// power

void integer_ring::pow(obj_t r, const obj_t a, ulong exp) const
{ fmpz_pow_ui( rop, op1, exp ); }

// compare

bool integer_ring::equal(const obj_t a, const obj_t b) const
{ return fmpz_equal( op1, op2 ); }

int  integer_ring::cmp(const obj_t a, const obj_t b) const
{ return fmpz_cmp( op1, op2 ); }
