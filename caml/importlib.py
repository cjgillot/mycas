header = r"""
#include <caml/mlvalues.h>
#include <caml/callback.h>

#define caml_callback0( f ) ( caml_callback( (f), Val_unit ) )
#define caml_callback1 caml_callback

"""

def prepare( s ):
  print>>s, header


#template = r"""
##define CALLER0( symbol, sig )            \
  #sig() {                                 \
    #extern void symbol();                 \
    #static void* val = &symbol;           \
    #caml_callback((value)&val, Val_unit); \
  #}

#CALLER0(camlTest__hello_1030, void hello)
#CALLER0(camlTest__hellos_1031, void hellos)

#int main(int argc, char **argv) {
  #size_t i;
  #caml_startup(argv);

  #for(i = 0; i < 1000; ++i)
    #hello();
  #printf("fini\n");
  #hellos();
  #return 0;
#}
#"""

def declare( s, func, symbol, arity ):
  print>>s, "value", func, "("

  if arity > 0:
    print>>s, "  value arg0"
    for i in range( 1, arity ):
      print>>s, ", value arg%u" % i

  print>>s, ") {"

  print>>s, "\textern void ", symbol, "();"
  print>>s, "\tstatic void* fnc = &", symbol, ";"

  if arity < 4:
    print>>s, "\tvalue ret = caml_callback%u"%arity, "( (value)&fnc"
    for i in range( 0, arity ):
      print>>s, "\t\t, arg%u" % i
    print>>s, "\t);"

  else:
    print>>s, "\tvalue args [ %u ] = {" % arity
    print>>s, "\t\t  arg0"
    for i in range( 1, arity ):
      print>>s, "\t\t, arg%u" % i
    print>>s, "\t};"
    print>>s, "\tvalue ret = caml_callbackN( (value)&fnc, %u, args );" % i

  print>>s, "}"
