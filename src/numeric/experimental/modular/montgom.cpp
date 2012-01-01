#include "numeric/modular/montgom.hpp"

using namespace numeric::detail_modular;

//! compute \f[ a*2^-n mod M \f]
//! assuming \f[ 0 <= a < 2^(2m) \f]
//! only rop <-> a alias is allowed
inline static void reduce( fmpz_t rop, const fmpz_t a, const fmpz_t M, ulong n, const fmpz_t V )
{
  fmpz_t ldb = { 0 };

  fmpz_fdiv_r_2exp( ldb, a, n );
  fmpz_fdiv_q_2exp( rop, a, n );

  fmpz_addmul( rop, V, ldb );
  fmpz_mod( rop, rop, M );

  fmpz_clear( ldb );
}

inline montgom_ring::montgom_ring(const fmpz_t M, ulong n_, ulong m_, const fmpz_t V_)
: basecase_ring( M ), m(m_), n(n_)
{
  fmpz_init( V );
  fmpz_set( V, V_ );
}

montgom_ring::~montgom_ring()
{
  fmpz_clear( V );
}

#define EMPTY
#define AS( c, o )  ( *reinterpret_cast<c fmpz_t*>(&o) )

#define op1 AS(const, a)
#define op2 AS(const, b)
#define rop AS(EMPTY, r)

#define M modulus

void montgom_ring::init( obj_t r, long v ) const
{
  *rop = 0;
  if( v )
    set( r, v );
}

void montgom_ring::set( obj_t r, long v ) const
{
  fmpz_set_si( rop, v );
  fmpz_mul_2exp( rop, rop, n );
  fmpz_mod( rop, rop, M );
}

bool montgom_ring::inv( obj_t r, const obj_t a ) const
{
  if( fmpz_is_zero( op1 ) )
    return false;
  if( ! fmpz_invmod( rop, op1, M ) )
    return false;
  // rop == ( a*N )^(-1) == a^-1 * N^-1
  // we must multiply by N^2
  fmpz_mul_2exp( rop, rop, n );
  fmpz_mul_2exp( rop, rop, n );
  fmpz_mod( rop, rop, M );
  return true;
}

// binary

void montgom_ring::mul(obj_t r, const obj_t a, const obj_t b) const
{
  fmpz_mul( rop, op1, op2 );
  reduce( rop, rop, M, n, V );
}

bool montgom_ring::div(obj_t r, const obj_t a, const obj_t b) const
{
  fmpz_t binv = { 0 };
  if( fmpz_is_zero( op2 ) )
    return false;
  if( ! fmpz_invmod( binv, op2, M ) )
    return false;
  // binv == ( b*N )^(-1) == b^-1 * N^-1
  fmpz_mul( rop, binv, op1 );
  // rop == b^-1 * N^-1 * a * N^-1 = a * b^-1
  // we must multiply by N
  fmpz_mul_2exp( rop, rop, n );
  fmpz_mod( rop, rop, M );
  return true;
}

// ternary

// void montgom_ring::addmul(obj_t r, const obj_t a, const obj_t b) const
// {
//   fmpz_addmul( rop, op1, op2 );
//   reduce( rop, rop, M, n, V );
// }
//
// void montgom_ring::submul(obj_t r, const obj_t a, const obj_t b) const
// {
//   fmpz_submul( rop, op1, op2 );
//   reduce( rop, rop, M, n, V );
// }

// power

bool montgom_ring::pow(obj_t r, const obj_t a, long exp) const
{
  // TODO
}
