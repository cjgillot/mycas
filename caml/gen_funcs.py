import sys
obj = sys.argv[1]
fun = sys.argv[2]

symbols = dict()

class symbol:
  def __init__(self, arity):
    self.arity = arity
    self.sym = symbol.counter = symbol.counter + 1
symbol.counter = -1

f = open( fun, "r" )

module = f.readline()
module = module[:-1]

import re
re1 = re.compile( r'(\d+)\s+(\w+)' )

for line in f:
  match = re1.match( line )
  arity = int( match.group(1) )
  symbols[ match.group(2) ] = symbol( arity )
f.close()
del f

native = False

cout = None
mlout = None

l = None

def parse_nm( obj, symbols ):
  from subprocess import *
  import re
  re2 = re.compile( r'\s*[0-9a-f]+ T (caml(\w+))' )

  pipe = Popen( [ "nm", "--defined-only", obj ], stdout=PIPE )

  for name in symbols.iterkeys():
    cont = True
    while cont:
      line = pipe.stdout.readline()

      match = re2.match( line )
      if match is None:
        continue

      search = match.group(1).find( name )
      if search < 0:
        continue

      symbols[name].sym = match.group(1)
      cont=False

if native:
  parse_nm( obj, symbols )

  import import_native
  l = import_native

  cout = open( fun + ".c", "w" )

else: # byte
  import import_byte
  l = import_byte

  cout = open( fun + ".c", "w" )
  mlout= open( fun + ".ml","w" )

l.prepare( cout, mlout, module, len( symbols ) )

for ( name, sym ) in symbols.iteritems():
  l.declare( cout, mlout, name, sym.sym, sym.arity )
