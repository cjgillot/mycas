#include "wordsize.hpp"

#include "util/compare.hpp"

#include <ulong_extras.h>

using namespace numeric::detail_modular;

wordsize_ring::wordsize_ring(ulong m)
{ nmod_init( &modulus, m ); }

wordsize_ring::~wordsize_ring() {}

#define rop  ( *reinterpret_cast<      mp_limb_t*>(r) )
#define op1  ( *reinterpret_cast<const mp_limb_t*>(a) )
#define op2  ( *reinterpret_cast<const mp_limb_t*>(b) )

// cdtor

void wordsize_ring::init(obj_t r, long v) const
{ NMOD_RED( rop, v, modulus ); }

void wordsize_ring::set (obj_t r, long v) const
{ NMOD_RED( rop, v, modulus ); }

void wordsize_ring::copy(obj_t r, const obj_t v) const
{ *r = *v; }

void wordsize_ring::clear(obj_t) const {}

// unary

void wordsize_ring::neg(obj_t r, const obj_t a) const
{ rop =  nmod_neg( op1, modulus ); }

bool wordsize_ring::inv(obj_t r, const obj_t a) const
{
  rop =  nmod_div( 1, op1, modulus );
#if 1
  if( r == a )
    return true;

  return nmod_mul( rop, op1, modulus ) == 1;
#else
  return true;
#endif
}

void wordsize_ring::incr(obj_t r, const obj_t a) const
{ rop = _nmod_add( op1, 1, modulus ); }

void wordsize_ring::decr(obj_t r, const obj_t a) const
{ rop = _nmod_sub( op1, 1, modulus ); }

// binary

void wordsize_ring::add(obj_t r, const obj_t a, const obj_t b) const
{ rop = _nmod_add( op1, op2, modulus ); }

void wordsize_ring::sub(obj_t r, const obj_t a, const obj_t b) const
{ rop = _nmod_sub( op1, op2, modulus ); }

void wordsize_ring::mul(obj_t r, const obj_t a, const obj_t b) const
{ rop =  nmod_mul( op1, op2, modulus ); }

bool wordsize_ring::div(obj_t r, const obj_t a, const obj_t b) const
{
  rop =  nmod_div( op1, op2, modulus );
#if 1
  if( r == a || r == b )
    return true;

  return nmod_mul( rop, op2, modulus ) == op1;
#else
  return true;
#endif
}

// ternary

void wordsize_ring::addmul(obj_t r, const obj_t a, const obj_t b) const
{ NMOD_ADDMUL( rop, op1, op2, modulus ); }

void wordsize_ring::submul(obj_t r, const obj_t a, const obj_t b) const
{ NMOD_ADDMUL( rop, op1, nmod_neg( op2, modulus ), modulus ); }

// power

bool wordsize_ring::pow(obj_t r, const obj_t b, long e) const
{
  rop = n_powmod2_preinv( op2, e, modulus.n, modulus.ninv );
  // TODO: test correct answer
  return true;
}

// compare

bool wordsize_ring::equal(const obj_t a, const obj_t b) const
{ return op1 == op2; }

int  wordsize_ring::cmp(const obj_t a, const obj_t b) const
{ return util::compare( op1, op2 ); }
