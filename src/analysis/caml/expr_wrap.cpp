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
  // TODO maybe adapt the used/max values to the stored expr ?
  return caml_alloc_custom( &expr_ops, sizeof( expr ), 0, 1000 );
}

#define expr_construct( e ) \
  new( Data_custom_val( e ) )

} // namespace _caml_expr

using _caml_expr::expr_allocate;

namespace analysis {

value expr_copy(const expr &e)
{
  value res = expr_allocate();
  expr_construct( res ) expr( e );
  return res;
}

#ifdef BOOST_HAS_RVALUE_REFS // c++0x
value expr_move(const expr&&e)
#else
value expr_move(const expr &e)
#endif
{
  value res = expr_allocate();
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

// for debugging
EXPORT value __caml_expr_print( value ex )
{
  Expr_val( ex )->print( std::cout );
  std::cout << std::flush;
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
  res = expr_move( std::move( e->diff( symbol(s), n ) ) );

  CAMLreturn( res );
}

EXPORT value __caml_expr_diff( value ex, value sym )
{
  CAMLparam2( ex, sym );
  const symbol_* s = static_cast< const symbol_* >( Expr_val( sym )->get() );
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


// unary operations
EXPORT value __caml_expr_neg( value e1 )
{
  CAMLparam1( e1 );
  const expr* a = Expr_val( e1 );

  CAMLlocal1( res );
  res = expr_move( std::move( a->neg() ) );

  CAMLreturn( res );
}
EXPORT value __caml_expr_inv( value e1 )
{
  CAMLparam1( e1 );
  const expr* a = Expr_val( e1 );

  CAMLlocal1( res );
  res = expr_move( std::move( a->inv() ) );

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
  res = expr_move( std::move( a->pow( *b ) ) );

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

#if 0
// for debugging purpose only
EXPORT value print_ws (value v) {
//   CAMLparam1(v);
  int taille,i ;
  if (Is_long(v)) printf("%d", Long_val(v));
  else {
    taille=Wosize_val(v);
    switch (Tag_val(v))
    {
    case String_tag :
      printf("\"%s\"", String_val(v));
      break;
    case Double_tag:
      printf("%g", Double_val(v));
      break;
    case Double_array_tag :
      printf ("[|");
              if (taille>0) printf("%g", Double_field(v,0));
      for (i=1;i<(taille/2);i++)  printf("; %g", Double_field(v,i));
      printf("|]");
      break;
    case Abstract_tag :
    case Custom_tag :
      printf("<abstract>");
      break;
    case Closure_tag :
      printf("<%d, ",Code_val(v)) ;
      if (taille>1) print_ws(Field(v,1)) ;
      for (i=2;i<taille;i++) {
        printf("; ") ;
        print_ws(Field(v,i));
      }
      printf(">");
      break;
    default:
      if (Tag_val(v)>=No_scan_tag) printf("?");
      else {
        printf("(");
        if (taille>0) print_ws(Field(v,0));
        for (i=1;i<taille;i++) {
          printf(", ");
          print_ws(Field(v,i));
        }
        printf(")");
      }
    }
  }
  fflush(stdout);
  return Val_unit;
}
#endif
