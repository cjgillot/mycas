AR=ar
CC=gcc
CXX=g++
RANLIB=ranlib
OCAMLOPT=ocamlopt.opt
OCAMLDEP=ocamldep.opt
OCAMLGENC=perl ../../../caml/gen_funcs.pl

INCLUDE=-I../../../ -I/usr/local/lib/ocaml
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


MLSRC =	\
	expr.ml		\
	base.ml		\
	aors.ml		\
	mul.ml		\
	newton.ml	\
	div.ml		\
	compose.ml	\
	taylor.ml	\
	end.ml		\
	pseries_export.ml

MLHDR = $(wildcard *.mli)

MLOBJ = $(MLSRC:.ml=.cmx)

PACK=Pseries_dense
PACKOBJ=pseries_dense.cmx

all: libpseries.a

libpseries.a: export.o $(PACKOBJ) ../closure.o
	$(OCAMLOPT) bigarray.cmxa $(PACKOBJ) -output-obj -o ml_arch.o
	$(AR) cr $@ export.o ml_arch.o ../closure.o
	$(RANLIB) $@

ml_arch.cmxa: $(MLOBJ)
	$(OCAMLOPT) $(MLOBJ) -a -o $@

mldepend.mk: $(MLSRC) $(MLHDR)
	$(OCAMLDEP) -native $(MLSRC) $(MLHDR) > mldepend.mk

-include mldepend.mk

.cpp.o:
	$(CXX) $(INCLUDE) $(CFLAGS) -c $< -o $@

.cc.o:
	$(CXX) $(INCLUDE) $(CFLAGS) -c $< -o $@

.ml.cmx:
	$(OCAMLOPT) $(MLFLAGS) -for-pack $(PACK) -c $<

.mli.cmi:
	$(OCAMLOPT) $(MLFLAGS) -for-pack $(PACK) -c $<

$(PACKOBJ): $(MLOBJ)
	$(OCAMLOPT) $(MLFLAGS) -pack -o $(PACKOBJ) $(MLOBJ)

export.o: export.cc
%.cc: ../%.def $(PACKOBJ)
	$(OCAMLGENC) $* $< $(PACKOBJ:.cmx=.o)

partialclean:
	rm -f *.o *.cm*
	rm -f export.def export.cc export.ml

clean: partialclean
	rm -f *.a
	rm -f cdepend.mk mldepend.mk

distclean: clean

.PHONY: partialclean clean distclean

.PRECIOUS: Makefile
