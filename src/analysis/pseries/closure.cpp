#include "analysis/pseries/closure.hpp"

#include "analysis/caml/expr_wrap.hpp"

#include <caml.hpp>

using namespace analysis;

struct map_closure_t {
  typedef expr (*fun_t)(const expr&, void*);

  fun_t func;
  void* data;
};

extern "C" CAMLprim value caml_call_map_closure( value closure, value arg )
{
  CAMLparam2( closure, arg );
  CAMLlocal1( ret );

  map_closure_t* fun = (map_closure_t*)closure;

  const expr re = (*fun->func)( *Expr_val(arg), fun->data );
  ret = expr_move( re );

  CAMLreturn( ret );
}
