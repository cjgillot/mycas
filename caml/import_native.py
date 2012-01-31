import import_common as ic

def prepare( s, ml, mod, sz ):
  ic.prepare_c( s )

def declare( s, _, func, symbol, arity ):
  ic.proto_c( s, func, arity )
  print>>s, "{"
  print>>s, "  extern void ", symbol, "();"
  print>>s, "  static void* fnc = &", symbol, ";"
  ic.call_c( s, arity, "(&fun)" )
  print>>s, "}"
