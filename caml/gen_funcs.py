from subprocess import *

import sys
obj = sys.argv[1]
fun = sys.argv[2]
out = sys.argv[3]

symbols = dict()

class symbol:
  def __init__(self, arity):
    self.arity = arity
    self.sym = None


f = open( fun, "r" )
for line in f:
  arity = int(line[0])
  symbols[ line[2:-1] ] = symbol( arity )
f.close()
del f

import re
regex = re.compile( r'\s*[0-9a-f]+ T (caml(\w+))' )

pipe = Popen( [ "nm", "--defined-only", obj ], stdout=PIPE )

for name in symbols.iterkeys():
  cont = True
  while cont:
    line = pipe.stdout.readline()

    match = regex.match( line )
    if match is None:
      continue

    search = match.group(1).find( name )
    if search < 0:
      continue

    symbols[name].sym = match.group(1)
    cont=False

out = open( out, "w" )

import importlib as l
l.prepare( out )

for ( name, sym ) in symbols.iteritems():
  l.declare( out, name, sym.sym, sym.arity )
