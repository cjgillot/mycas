#include <caml.hpp>

#define EXPORT extern "C"

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"

#include "analysis/caml/expr_wrap.hpp"

using namespace analysis;

namespace _caml_expr {

static int
expr_compare( value a, value b )
{ return expr::compare( *Expr_val(a), *Expr_val(b) ); }

static intnat
expr_hash( value a )
{ return Expr_val(a)->hash(); }

static void
expr_finalize( value ex )
{
  const expr *e = Expr_val( ex );
  e->~expr();
}

struct custom_operations expr_ops = {
  (char*)"analysis.expr",
  &expr_finalize,
  &expr_compare,
  &expr_hash,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default
};

value expr_allocate()
{
#define wosize ( 2 + (sizeof(expr) + sizeof(value) - 1) / sizeof(value) )
  value ret;
  if( wosize < Max_young_wosize )
    ret = caml_alloc_small( wosize, Custom_tag );
  else
    ret = caml_alloc_shr( wosize, Custom_tag );
  Field( ret, 0 ) = (value)&expr_ops;
  return ret;
}

#define expr_construct( e ) \
  new( (void*)Expr_val((e)) )

} // namespace _caml_expr

using _caml_expr::expr_allocate;

namespace analysis {

value expr_copy(const expr &e)
{
  value res = expr_allocate();
  Field( res, 1 ) = RTTI_ID( e.get() );
  expr_construct( res ) expr( e );
  return res;
}

value expr_move(expr&&e)
{
  value res = expr_allocate();
  Field( res, 1 ) = RTTI_ID( e.get() );
  expr_construct( res ) expr( std::move(e) );
  return res;
}

} // namespace analysis

using namespace analysis;

EXPORT value __caml_expr_new_of_int( value arg )
{
  CAMLparam1( arg );
  CAMLlocal1( res );

  int n = Int_val( arg );

  res = expr_allocate();
  Field( res, 1 ) = RTTI_TYPE_ID( numerical );
  expr_construct( res ) expr( n );

  CAMLreturn( res );
}

// test
EXPORT value __caml_expr_null( value ex )
{
  return Val_bool( Expr_val( ex )->null() );
}

EXPORT value __caml_expr_unit( value ex )
{
  return Val_bool( Expr_val( ex )->unit() );
}

// for debugging
EXPORT value __caml_expr_print( value ex )
{
  Expr_val( ex )->print( std::cout );
  std::cout << std::flush;
  return Val_unit;
}


EXPORT value __caml_expr_has( value ex, value sym )
{
  const symbol_* s = Expr_val( sym )->as_a<symbol_>();
  Expr_val( ex )->has( symbol(s) );
  return Val_unit;
}

EXPORT value __caml_expr_diff_n( value ex, value sym, value order )
{
  CAMLparam3( ex, sym, order );
  const symbol_* s = Expr_val( sym )->as_a<symbol_>();
  const expr   * e = Expr_val( ex );
  long n = Int_val( order );

  CAMLlocal1( res );
  res = expr_move( std::move( e->diff( symbol(s), n ) ) );

  CAMLreturn( res );
}

EXPORT value __caml_expr_diff( value ex, value sym )
{
  CAMLparam2( ex, sym );
  const symbol_* s = Expr_val( sym )->as_a<symbol_>();
  const expr   * e = Expr_val( ex );

  CAMLlocal1( res );
  res = expr_move( std::move( e->diff( symbol(s) ) ) );

  CAMLreturn( res );
}

EXPORT value __caml_expr_subs( value ex, value rules_array )
{
  CAMLparam2( ex, rules_array );
  const expr* e = Expr_val( ex );

  exmap rules;
  std::size_t n = Wosize_val( rules_array );
  for( std::size_t i = 0; i < n; ++i )
  {
    value rule = Field( rules_array, i );
    const expr* s = Expr_val( Field( rule, 0 ) );
    const expr* x = Expr_val( Field( rule, 1 ) );

    rules.insert( std::make_pair( *s, *x ) );
  }

  CAMLlocal1( res );
  res = expr_move( std::move( e->subs( rules ) ) );

  CAMLreturn( res );
}

EXPORT value __caml_expr_expand( value ex )
{
  CAMLparam1( ex );
  const expr* e = Expr_val( ex );

  CAMLlocal1( res );
  res = expr_move( std::move( e->expand() ) );

  CAMLreturn( res );
}

EXPORT value __caml_expr_series( value ex, value sym )
{
  CAMLparam2( ex, sym );
  const symbol_* s = Expr_val( sym )->as_a<symbol_>();
  const expr   * e = Expr_val( ex );

  CAMLlocal1( res );
  res = expr_move( std::move( e->series( symbol(s) ) ) );

  CAMLreturn( res );
}

// TODO match & series

// unary operations
EXPORT value __caml_expr_neg( value e1 )
{
  CAMLparam1( e1 );
  const expr* a = Expr_val( e1 );

  CAMLlocal1( res );
  res = expr_move( std::move( expr::neg( *a ) ) );

  CAMLreturn( res );
}
EXPORT value __caml_expr_inv( value e1 )
{
  CAMLparam1( e1 );
  const expr* a = Expr_val( e1 );

  CAMLlocal1( res );
  res = expr_move( std::move( expr::inv( *a ) ) );

  CAMLreturn( res );
}

// binary operations
EXPORT value __caml_expr_add( value e1, value e2 )
{
  CAMLparam2( e1, e2 );
  const expr* a = Expr_val( e1 );
  const expr* b = Expr_val( e2 );

  CAMLlocal1( res );
  res = expr_move( std::move( expr::add( *a, *b ) ) );

  CAMLreturn( res );
}
EXPORT value __caml_expr_sub( value e1, value e2 )
{
  CAMLparam2( e1, e2 );
  const expr* a = Expr_val( e1 );
  const expr* b = Expr_val( e2 );

  CAMLlocal1( res );
  res = expr_move( std::move( expr::sub( *a, *b ) ) );

  CAMLreturn( res );
}
EXPORT value __caml_expr_mul( value e1, value e2 )
{
  CAMLparam2( e1, e2 );
  const expr* a = Expr_val( e1 );
  const expr* b = Expr_val( e2 );

  CAMLlocal1( res );
  res = expr_move( std::move( expr::mul( *a, *b ) ) );

  CAMLreturn( res );
}
EXPORT value __caml_expr_div( value e1, value e2 )
{
  CAMLparam2( e1, e2 );
  const expr* a = Expr_val( e1 );
  const expr* b = Expr_val( e2 );

  CAMLlocal1( res );
  res = expr_move( std::move( expr::div( *a, *b ) ) );

  CAMLreturn( res );
}

// power
EXPORT value __caml_expr_pow( value e1, value e2 )
{
  CAMLparam2( e1, e2 );
  const expr* a = Expr_val( e1 );
  const expr* b = Expr_val( e2 );

  CAMLlocal1( res );
  res = expr_move( std::move( expr::pow( *a, *b ) ) );

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
