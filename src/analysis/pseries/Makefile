SUBDIRS=dense sparse general

AR=ar
CC=gcc
CXX=g++
RANLIB=ranlib
OCAMLOPT=ocamlopt.opt
OCAMLDEP=ocamldep.opt
OCAMLGENH=perl ../../caml/gen_decls.pl
OCAMLGENC=perl ../../caml/gen_funcs.pl

INCLUDE=-I../../ -I/usr/local/lib/ocaml
CFLAGS =
MLFLAGS=

ifdef DEBUG
CFLAGS  += -g
MLFLAGS += -g -inline 0
endif

LDFLAGS=-L/usr/local/lib/ocaml -lasmrun -ldl -lm -lunix

.SUFFIXES :
.SUFFIXES : .cpp .cc .o .def
.SUFFIXES : .ml .mli .cmx .cmi

all: export.h libpseries.a

libpseries.a: dense/libpseries.a
	ln -s dense/libpseries.a libpseries.a

%/libpseries.a:
	$(MAKE) -C $*

export.o: export.cc export.h
%.h: %.def
	$(OCAMLGENH) $* $<

%/partialclean:
	$(MAKE) -C $* partialclean
%/clean:
	$(MAKE) -C $* clean
%/distclean:
	$(MAKE) -C $* distclean

partialclean: dense/partialclean
	rm -f libpseries.a export.h
	rm -f *.o *.a

clean: partialclean dense/clean

distclean: clean dense/distclean

.PHONY: partialclean clean distclean

.PRECIOUS: Makefile
