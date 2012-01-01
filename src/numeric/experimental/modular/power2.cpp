#include "power2.hpp"

#include "util/compare.hpp"

using namespace numeric::detail_modular;

typedef unsigned long ulong;

power2_ring::power2_ring(ulong m)
: mask(m)
{ assert( ( m & ( m - 1 ) ) == 0 ); }

power2_ring::~power2_ring() {}

#define rop  ( *reinterpret_cast<      ulong*>(r) )
#define op1  ( *reinterpret_cast<const ulong*>(a) )
#define op2  ( *reinterpret_cast<const ulong*>(b) )

// cdtor

void power2_ring::init(obj_t r, long v) const
{ rop = v; }

void power2_ring::set (obj_t r, long v) const
{ rop = v; }

void power2_ring::copy(obj_t r, const obj_t a) const
{ rop = op1; }

void power2_ring::clear(obj_t) const {}

// unary

void power2_ring::neg(obj_t r, const obj_t a) const
{ rop = - op1; }

inline static ulong inv_2adic(register ulong x)
{
  /*! 2-adic inversion
   *
   *  Theorem :
   *    If    x.y = 1 (mod p^k),
   *    Then  x.y.(2-x.y) = 1 (mod p^(2k))
   */

  register unsigned long
    i = 1, // inverse
    p = x; // = i * x

  // # correct bits doubles each time
  // so loop executes log(wordsize) times
  while(p != 1)
  {
    i *= 2 - p;
    p = i * x;
  }

  return i;
}

bool power2_ring::inv(obj_t r, const obj_t a) const
{
  if( ( rop & 1 ) == 0 )
    return false;

  rop = inv_2adic( op1 );

  return true;
}

void power2_ring::incr(obj_t r, const obj_t a) const
{ rop = op1 + 1; }

void power2_ring::decr(obj_t r, const obj_t a) const
{ rop = op1 - 1; }

// binary

void power2_ring::add(obj_t r, const obj_t a, const obj_t b) const
{ rop = op1 + op2; }

void power2_ring::sub(obj_t r, const obj_t a, const obj_t b) const
{ rop = op1 - op2; }

void power2_ring::mul(obj_t r, const obj_t a, const obj_t b) const
{ rop = op1 * op2; }

bool power2_ring::div(obj_t r, const obj_t a, const obj_t b) const
{
  if( ( op2 & 1 ) == 0 )
    return false;

  rop = op1 * inv_2adic(op2);
  return true;
}

// ternary

void power2_ring::addmul(obj_t r, const obj_t a, const obj_t b) const
{ rop += op1 * op2; }

void power2_ring::submul(obj_t r, const obj_t a, const obj_t b) const
{ rop -= op1 * op2; }

// power

inline static ulong pow2( ulong x, ulong e )
{
  ulong r = 1;

  while( e )
  {
    if( e & 1 ) r *= x;

    x *= x; e >>= 1;
  }

  return r;
}

bool power2_ring::pow(obj_t r, const obj_t a, long e) const
{
  if( e > 0 )
    rop = pow2( op1, e );

  else if( e < 0 )
  {
    if( (op1 & 1) == 0 )
      return false;
    rop = pow2( inv_2adic( op1 ), - e );
  }

  else
    rop = 1;

  return true;
}

// compare

bool power2_ring::equal(const obj_t a, const obj_t b) const
{
  *const_cast<ulong*>( &op1 ) &= mask;
  *const_cast<ulong*>( &op2 ) &= mask;
  return op1 == op2;
}

int  power2_ring::cmp(const obj_t a, const obj_t b) const
{
  *const_cast<ulong*>( &op1 ) &= mask;
  *const_cast<ulong*>( &op2 ) &= mask;
  return util::compare( op1, op2 );
}
