#include "wordsize.hpp"

#include "util/compare.hpp"

#include <ulong_extras.h>

using namespace numeric::detail;

wordsize_ring::
wordsize_ring(ulong m) {
  nmod_init( &modulus, m );
}

wordsize_ring::
~wordsize_ring() {}

#define ASM( o )  ( *reinterpret_cast<mp_limb_t*>(o) )
#define ASC( o )  ( *reinterpret_cast<const mp_limb_t*>(o) )

// cdtor

void wordsize_ring::init(obj_t o, long v) const
{ NMOD_RED( ASM(o), v, modulus ); }

void wordsize_ring::set (obj_t o, long v) const
{ NMOD_RED( ASM(o), v, modulus ); }

void wordsize_ring::copy(obj_t o, const obj_t v) const
{ *o = *v; }

void wordsize_ring::clear(obj_t) const {}

// unary

void wordsize_ring::neg(obj_t r, const obj_t a) const
{ ASM(r) =  nmod_neg( ASC(a), modulus ); }

void wordsize_ring::inv(obj_t r, const obj_t a) const
{ ASM(r) =  nmod_div( 1, ASC(a), modulus ); }

void wordsize_ring::incr(obj_t r, const obj_t a) const
{ ASM(r) = _nmod_add( ASC(a), 1, modulus ); }

void wordsize_ring::decr(obj_t r, const obj_t a) const
{ ASM(r) = _nmod_sub( ASC(a), 1, modulus ); }

// binary

void wordsize_ring::add(obj_t r, const obj_t a, const obj_t b) const
{ ASM(r) = _nmod_add( ASC(a), ASC(b), modulus ); }

void wordsize_ring::sub(obj_t r, const obj_t a, const obj_t b) const
{ ASM(r) = _nmod_sub( ASC(a), ASC(b), modulus ); }

void wordsize_ring::mul(obj_t r, const obj_t a, const obj_t b) const
{ ASM(r) =  nmod_mul( ASC(a), ASC(b), modulus ); }

void wordsize_ring::div(obj_t r, const obj_t a, const obj_t b) const
{ ASM(r) =  nmod_div( ASC(a), ASC(b), modulus ); }

// ternary

void wordsize_ring::addmul(obj_t r, const obj_t a, const obj_t b) const
{ NMOD_ADDMUL( ASM(r), ASC(a), ASC(b), modulus ); }

void wordsize_ring::submul(obj_t r, const obj_t a, const obj_t b) const
{ NMOD_ADDMUL( ASM(r), ASC(a), nmod_neg( ASC(b), modulus ), modulus ); }

// power

void wordsize_ring::pow(obj_t r, const obj_t b, long e) const
{ ASM(r) = n_powmod2_preinv( ASC(b), e, modulus.n, modulus.ninv ); }

// compare

bool wordsize_ring::equal(const obj_t a, const obj_t b) const
{ return ASC(a) == ASC(b); }

int  wordsize_ring::cmp(const obj_t a, const obj_t b) const
{ return util::compare( ASC(a), ASC(b) ); }
