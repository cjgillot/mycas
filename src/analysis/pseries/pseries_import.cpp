#include "caml_support.hpp"

#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/custom.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>

#define EXPORT extern "C" CAMLprim

#include "analysis/expr.ipp"
#include "analysis/symbol.hpp"

#include "util/move.hpp"

#define Expr_val( x ) ( reinterpret_cast< const expr* >( Data_custom_val( (x) ) ) )

#include "analysis/pseries/pseries_import.hpp"

using namespace analysis;
using namespace pseries_detail;

namespace _caml_expr {

extern value expr_allocate();

#define expr_construct( e ) \
  new( Data_custom_val( e ) )

}

using _caml_expr::expr_allocate;

enum {
  ps_empty=0

, ps_make_alone
, ps_make_array

, ps_map

, ps_neg
, ps_inv

, ps_add
, ps_sub
, ps_mul
, ps_div

, ps_max
};

struct function_locker
{
  value funcs[ ps_max ];

  function_locker()
  {
    caml::initializer_t::instance.touch();
#ifndef NDEBUG
    std::memset( funcs, 0, ps_max );
#endif
  }
  ~function_locker()
  {
    std::size_t n = ps_max;
    while( n != 0 )
      caml_remove_generational_global_root( funcs + --n );
  }

  void touch() const {}
};
static function_locker locker;

EXPORT value __caml_register( value fun, value index )
{
  locker.touch();

  std::size_t n = Int_val( index );
  locker.funcs[ n ] = fun;
  caml_register_generational_global_root( locker.funcs + n );

  return Val_unit;
}

EXPORT value __caml_call_c_closure( value fun, value arg )
{
  closure_t* f = (closure_t*)fun;
  (*f->func)( *Expr_val( arg ), f->data );
  return Val_unit;
}

repr::repr()
: m_value( (void*)locker.funcs[ ps_empty ] )
{}

repr::repr(const expr &e)
{
  value v = locker.funcs[ ps_make_alone ];

  ASSERT( v != Val_unit );

  value ex = expr_allocate();
  expr_construct( ex ) expr( e );

  m_value = (void*)caml_callback( v, (value)ex );
  caml_register_global_root( (value*)&m_value );
}

repr::~repr()
{
  caml_remove_global_root( (value*)&m_value );
}

repr::repr( void* val )
: m_value(val)
{
  caml_register_global_root( (value*)&m_value );
}

void repr::map_closure(closure_t* fun)
{
  value v = locker.funcs[ ps_map ];

  ASSERT( v != Val_unit );

  caml_callback2( v, (value)fun, (value)m_value );
}

repr* repr::neg(const repr &s)
{
  value v = locker.funcs[ ps_neg ];

  ASSERT( v != Val_unit );

  value ret = caml_callback( v, (value)s.m_value );
  return new repr( (void*)ret );
}

repr* repr::inv(const repr &s)
{
  value v = locker.funcs[ ps_inv ];

  ASSERT( v != Val_unit );

  value ret = caml_callback( v, (value)s.m_value );
  return new repr( (void*)ret );
}

repr* repr::add(const repr &a, const repr &b)
{
  value v = locker.funcs[ ps_add ];

  ASSERT( v != Val_unit );

  value ret = caml_callback2( v, (value)a.m_value, (value)b.m_value );
  return new repr( (void*)ret );
}

repr* repr::sub(const repr &a, const repr &b)
{
  value v = locker.funcs[ ps_sub ];

  ASSERT( v != Val_unit );

  value ret = caml_callback2( v, (value)a.m_value, (value)b.m_value );
  return new repr( (void*)ret );
}

repr* repr::mul(const repr &a, const repr &b)
{
  value v = locker.funcs[ ps_mul ];

  ASSERT( v != Val_unit );

  value ret = caml_callback2( v, (value)a.m_value, (value)b.m_value );
  return new repr( (void*)ret );
}

repr* repr::div(const repr &a, const repr &b)
{
  value v = locker.funcs[ ps_div ];

  ASSERT( v != Val_unit );

  value ret = caml_callback2( v, (value)a.m_value, (value)b.m_value );
  return new repr( (void*)ret );
}
