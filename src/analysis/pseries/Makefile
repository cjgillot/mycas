MLSRC =	\
	expr.ml		\
	dllist.ml	\
	pseries_base.ml	\
	aors.ml		\
	mul.ml		\
	newton.ml	\
	div.ml		\
	compose.ml	\
	taylor.ml	\
	pseries_end.ml	\
	pseries_export.ml

MLHDR = $(wildcard *.mli)

MLOBJ = $(MLSRC:.ml=.cmx)

AR=ar
CC=gcc
CXX=g++
RANLIB=ranlib
OCAMLOPT=ocamlopt.opt
OCAMLDEP=ocamldep.opt
OCAMLGENH=perl ../../caml/gen_decls.pl
OCAMLGENC=perl ../../caml/gen_funcs.pl

INCLUDE=-I../../ -I/usr/local/lib/ocaml
CFLAGS =-g -fPIC
MLFLAGS=-g -fPIC

LDFLAGS=-L/usr/local/lib/ocaml -lasmrun -ldl -lm -lunix

.SUFFIXES :
.SUFFIXES : .cpp .cc .o .def
.SUFFIXES : .ml .mli .cmx .cmi

all: export.h libpseries.a

libpseries.a: export.o pseries.cmx closure.o
	$(OCAMLOPT) bigarray.cmxa pseries.cmx -output-obj -o ml_arch.o
# 	cp ml_arch.a $@
	$(AR) cr $@ export.o ml_arch.o closure.o
	$(RANLIB) libpseries.a

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
	$(OCAMLOPT) $(MLFLAGS) -for-pack Pseries -c $<

.mli.cmi:
	$(OCAMLOPT) $(MLFLAGS) -for-pack Pseries -c $<

pseries.cmx: $(MLOBJ)
	$(OCAMLOPT) $(MLFLAGS) -pack -o pseries.cmx $(MLOBJ)

export.o: export.cc export.h
%.h: %.def
	$(OCAMLGENH) $* $<
%.cc: %.def pseries.cmx
	$(OCAMLGENC) $* $< pseries.o

partialclean:
	rm -f *.o *.cm*
	rm -f export.cc export.ml

clean: partialclean
	rm -f *.a
	rm -f cdepend.mk mldepend.mk

distclean: clean
	rm -f export.h

.PHONY: partialclean clean distclean

.PRECIOUS: Makefile
