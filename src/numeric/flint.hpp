#ifndef FLINT_HPP
#define FLINT_HPP

#include <mpir.h>
#include <mpfr.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#undef ulong

#endif
