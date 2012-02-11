CSRC = $(wildcard *.cpp)
CHDR = $(wildcard *.hpp)

COBJ = $(CSRC:.cpp=.o)

MLSRC= $(wildcard *.ml)
MLHDR= $(wildcard *.mli)

MLOBJ= $(MLSRC:.ml=.cmx)

AR=ar
CXX=g++
RANLIB=ranlib
OCAMLOPT=ocamlopt.opt
OCAMLDEP=ocamldep.opt
OCAMLGEN=perl ../../caml/gen_funcs.pl

INCLUDE= -I../../ -I/usr/local/lib/ocaml
CPPFLAGS= -g

.SUFFIXES :
.SUFFIXES : .cpp .hpp .o
.SUFFIXES : .ml .mli .cmx .cmi

all: libpseries.a

libpseries.a: $(COBJ) ml_arch.cmxa
	cp ml_arch.a $@
	$(AR) r $@ $(COBJ)
	$(RANLIB) libpseries.a

ml_arch.cmxa: $(MLOBJ)
	$(OCAMLOPT) -a -o $@ $(MLOBJ)

mldepend.mk: $(MLSRC) $(MLHDR)
	$(OCAMLDEP) -native $(MLSRC) $(MLHDR) > mldepend.mk

cdepend.mk: $(CSRC) $(CHDR) export.h
	$(CXX) -E -MM $(CSRC) $(INCLUDE) > cdepend.mk

-include mldepend.mk
-include cdepend.mk

.c.o:
	$(CC)  $(INCLUDE) $(CPPFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(INCLUDE) $(CPPFLAGS) -c $< -o $@

.ml.cmx:
	$(OCAMLOPT) $(MLFLAGS) -c $<

.mli.cmi:
	$(OCAMLOPT) $(MLFLAGS) -c $<

export.h: export.c
export.o: export.c
%.c: %.def pseries_export.cmx
	$(OCAMLGEN) $* $< pseries_export.o

clean:
	rm -f *.o *.cm* *.a
	rm -f *.def.*
	rm -f cdepend.mk mldepend.mk
	rm -f export.c export.ml

distclean: clean
	rm -f export.h

.PHONY: clean distclean

.PRECIOUS: Makefile
