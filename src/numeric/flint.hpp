#ifndef FLINT_HPP
#define FLINT_HPP

#include <mpir.h>
#include <mpfr.h>

#ifdef __cplusplus
#define GUARD_BEGIN extern "C" {
#define GUARD_END   }
#else
#define GUARD_BEGIN /**/
#define GUARD_END   /**/
#endif

GUARD_BEGIN

#ifdef FLINT_H
#error "DO NOT INCLUDE FLINT.H BEFORE THIS FILE"
#endif

#include <flint.h>

#include <fmpz.h>
#include <fmpz_vec.h>
#include <fmpz_mat.h>
#include <fmpz_poly.h>
#include <fmpz_factor.h>
#include <fmpz_poly_q.h>
#include <fmpz_poly_mat.h>
#include <fmpz_mod_poly.h>

#include <nmod_vec.h>
#include <nmod_mat.h>
#include <nmod_poly.h>
#include <nmod_poly_mat.h>

#include <fmpq.h>
#include <fmpq_mat.h>
#include <fmpq_poly.h>

/* double */
void fmpz_set_d(fmpz_t, double);
void fmpq_set_d(fmpq_t, double);
double fmpz_get_d(const fmpz_t);
double fmpq_get_d(const fmpq_t);

/* cmp */
int fmpz_cmp_d(const fmpz_t, double);

int fmpz_cmpabs_ui(const fmpz_t, ulong);

int fmpq_cmp(const fmpq_t, const fmpq_t);
int fmpq_cmp_ui(const fmpq_t, ulong, ulong);
int fmpq_cmp_si(const fmpq_t,  long, ulong);

/* set */
#define STR(x) (const_cast<char*>(x))
// int fmpz_set_str(fmpz_t, const char*, int);
int  fmpq_set_str(fmpq_t, const char*, int);
void fmpz_set_q(fmpz_t, const fmpq_t);
void mpfr_set_fz(mpfr_ptr, const fmpz_t, mpfr_rnd_t);

/* bitwise */
void fmpz_com(fmpz_t, const fmpz_t);
void fmpz_and(fmpz_t, const fmpz_t, const fmpz_t);
void fmpz_ior(fmpz_t, const fmpz_t, const fmpz_t);
void fmpz_xor(fmpz_t, const fmpz_t, const fmpz_t);

/* hash */
size_t fmpz_hash(const fmpz_t);
size_t fmpq_hash(const fmpq_t);
size_t mpfr_hash(mpfr_srcptr);

GUARD_END

#undef GUARD_BEGIN
#undef GUARD_END

#undef ulong

#endif
