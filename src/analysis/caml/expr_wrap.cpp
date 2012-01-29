#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/custom.h>
#include <caml/mlvalues.h>

#define EXPORT extern "C"

#include "analysis/expr.ipp"
#include "analysis/symbol.hpp"

#include "util/move.hpp"

#define Expr_val( x ) ( reinterpret_cast< const expr* >( Data_custom_val( (x) ) ) )

using namespace analysis;

namespace _caml_expr {

static void expr_finalize( value ex )
{
  const expr *e = Expr_val( ex );
  e->~expr();
}

struct custom_operations expr_ops = {
  "analysis.expr",
  &expr_finalize,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default
};

value expr_allocate()
{
  return caml_alloc_custom( &expr_ops, sizeof( expr ), 0, 1000 );
}

#define expr_construct( e ) \
  new( Data_custom_val( e ) )

}

using _caml_expr::expr_allocate;

EXPORT value __caml_expr_new_of_int( value arg )
{
  CAMLparam1( arg );
  CAMLlocal1( res );

  int n = Int_val( arg );

  res = expr_allocate();
  expr_construct( res ) expr( n );

  CAMLreturn( res );
}


EXPORT value __caml_expr_null( value ex )
{
  return Val_bool( Expr_val( ex )->null() );
}

EXPORT value __caml_expr_unit( value ex )
{
  return Val_bool( Expr_val( ex )->unit() );
}


EXPORT value __caml_expr_print( value ex, value stream )
{
  Expr_val( ex )->print( *reinterpret_cast< std::ostream* >( stream ) );
  return Val_unit;
}


EXPORT value __caml_expr_has( value ex, value sym )
{
  const symbol_* s = static_cast< const symbol_* >( Expr_val( sym )->get() );
  Expr_val( ex )->has( symbol(s) );
  return Val_unit;
}

EXPORT value __caml_expr_diff_n( value ex, value sym, value order )
{
  CAMLparam3( ex, sym, order );
  const symbol_* s = static_cast< const symbol_* >( Expr_val( sym )->get() );
  const expr   * e = Expr_val( ex );
  long n = Int_val( order );

  CAMLlocal1( res );
  res = expr_allocate();
  expr_construct( res ) expr( std::move( e->diff( symbol(s), n ) ) );

  CAMLreturn( res );
}

EXPORT value __caml_expr_diff( value ex, value sym )
{
  CAMLparam2( ex, sym );
  const symbol_* s = static_cast< const symbol_* >( Expr_val( sym )->get() );
  const expr   * e = Expr_val( ex );

  CAMLlocal1( res );
  res = expr_allocate();
  expr_construct( res ) expr( std::move( e->diff( symbol(s) ) ) );

  CAMLreturn( res );
}


EXPORT value __caml_expr_expand( value ex )
{
  CAMLparam1( ex );
  const expr* e = Expr_val( ex );

  CAMLlocal1( res );
  res = expr_allocate();
  expr_construct( res ) expr( std::move( e->expand() ) );

  CAMLreturn( res );
}


// unary operations
EXPORT value __caml_expr_neg( value e1 )
{
  CAMLparam1( e1 );
  const expr* a = Expr_val( e1 );

  CAMLlocal1( res );
  res = expr_allocate();
  expr_construct( res ) expr( std::move( a->neg() ) );

  CAMLreturn( res );
}
EXPORT value __caml_expr_inv( value e1 )
{
  CAMLparam1( e1 );
  const expr* a = Expr_val( e1 );

  CAMLlocal1( res );
  res = expr_allocate();
  expr_construct( res ) expr( std::move( a->inv() ) );

  CAMLreturn( res );
}

// binary operations
EXPORT value __caml_expr_add( value e1, value e2 )
{
  CAMLparam2( e1, e2 );
  const expr* a = Expr_val( e1 );
  const expr* b = Expr_val( e2 );

  CAMLlocal1( res );
  res = expr_allocate();
  expr_construct( res ) expr( std::move( expr::add( *a, *b ) ) );

  CAMLreturn( res );
}
EXPORT value __caml_expr_sub( value e1, value e2 )
{
  CAMLparam2( e1, e2 );
  const expr* a = Expr_val( e1 );
  const expr* b = Expr_val( e2 );

  CAMLlocal1( res );
  res = expr_allocate();
  expr_construct( res ) expr( std::move( expr::sub( *a, *b ) ) );

  CAMLreturn( res );
}
EXPORT value __caml_expr_mul( value e1, value e2 )
{
  CAMLparam2( e1, e2 );
  const expr* a = Expr_val( e1 );
  const expr* b = Expr_val( e2 );

  CAMLlocal1( res );
  res = expr_allocate();
  expr_construct( res ) expr( std::move( expr::mul( *a, *b ) ) );

  CAMLreturn( res );
}
EXPORT value __caml_expr_div( value e1, value e2 )
{
  CAMLparam2( e1, e2 );
  const expr* a = Expr_val( e1 );
  const expr* b = Expr_val( e2 );

  CAMLlocal1( res );
  res = expr_allocate();
  expr_construct( res ) expr( std::move( expr::div( *a, *b ) ) );

  CAMLreturn( res );
}

// power
EXPORT value __caml_expr_pow( value e1, value e2 )
{
  CAMLparam2( e1, e2 );
  const expr* a = Expr_val( e1 );
  const expr* b = Expr_val( e2 );

  CAMLlocal1( res );
  res = expr_allocate();
  expr_construct( res ) expr( std::move( a->pow( *b ) ) );

  CAMLreturn( res );
}

// compare
EXPORT value __caml_expr_compare( value e1, value e2 )
{
  const expr* a = Expr_val( e1 );
  const expr* b = Expr_val( e2 );

  return Val_bool( expr::compare( *a, *b ) );
}

EXPORT value __caml_expr_hash( value ex )
{
  const expr* e = Expr_val( ex );

  return Val_int( e->hash() );
}
