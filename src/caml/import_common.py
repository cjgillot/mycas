mlheader = r"""
open %s

external export : 'a -> int -> unit = "__export_functions_%s"
"""

cheader = r"""
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/memory.h>

#define caml_callback0( f ) ( caml_callback( (f), Val_unit ) )
#define caml_callback1 caml_callback
"""

cmain = """{
  CAMLparam2( fun, index );
  size_t n = Int_val( index );
  fun_table[n] = fun;
  caml_register_generational_global_root( &fun_table[n] );
  CAMLreturn Val_unit;
}
"""

def prepare_c( s ):
  print>>s,  cheader

def proto_c( s, func, arity ):
  # C side
  print>>s, "value", func, "("

  if arity > 0:
    print>>s, "  value arg0"
    for i in range( 1, arity ):
      print>>s, ", value arg%u" % i

  print>>s, ")"

def call_c( s, arity, symbol ):
  if arity < 4:
    print>>s, "  value ret = caml_callback%u"%arity, "( (value)", symbol
    for i in range( 0, arity ):
      print>>s, "    , arg%u" % i
    print>>s, "  );"

  else:
    print>>s, "  value args [", arity, "] = {"
    print>>s,   "      arg0"
    for i in range( 1, arity ):
      print>>s, "    , arg%u" % i
    print>>s, "  };"
    print>>s, "  value ret = caml_callbackN( (value)", symbol, ", ", arity, ", args );"

  print>>s, "return ret;"
