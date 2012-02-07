import import_common as ic

cexport = """{
  CAMLparam2( fun, index );
  size_t n = Int_val( index );
  fun_table[n] = fun;
  caml_register_generational_global_root( &fun_table[n] );
  CAMLreturn( Val_unit );
}
"""

def prepare( s, ml, module, size ):
  print>>ml, "open", module.capitalize()
  print>>ml, "external export : 'a -> int -> unit = ", "\"__export_functions_%s\"" % module

  ic.prepare_c( s )
  print>>s, "static value fun_table[", size, "] = { 0", (size-1)*", 0", "};"
  print>>s, "CAMLprim value", "__export_functions_%s" % module, "( value fun, value index )"
  print>>s, cexport

def declare( s, ml, func, index, arity ):
  # C side
  ic.proto_c( s, func, arity )
  print>>s, "{"
  ic.call_c( s, arity, "(fun_table[ %u ])"%index )
  print>>s, "}"

  # Caml side
  print>>ml, "let () = export ", func, index
