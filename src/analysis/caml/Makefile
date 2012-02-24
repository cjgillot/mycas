AR=ar
CC=gcc
CXX=g++
RANLIB=ranlib
OCAMLOPT=ocamlopt.opt
OCAMLDEP=ocamldep.opt

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


MLSRC =	\
	rtti.ml		\
	expr.ml		\
	iterator.ml	\
	match.ml

MLHDR = $(wildcard *.mli)

MLOBJ = $(MLSRC:.ml=.cmx)

all: libexpr_wrap.a

libexpr_wrap.a: $(MLOBJ)
	$(OCAMLOPT) $(MLOBJ) -output-obj -o expr_wrap.o
	$(AR) cr $@ expr_wrap.o
	$(RANLIB) $@

mldepend.mk: $(MLSRC) $(MLHDR)
	$(OCAMLDEP) -native $(MLSRC) $(MLHDR) > mldepend.mk

rtti.ml: ../rtti.ml
	cp $< $@

-include mldepend.mk

.cpp.o:
	$(CXX) $(INCLUDE) $(CFLAGS) -c $< -o $@

.cc.o:
	$(CXX) $(INCLUDE) $(CFLAGS) -c $< -o $@

.ml.cmx:
	$(OCAMLOPT) $(MLFLAGS) -c $<

.mli.cmi:
	$(OCAMLOPT) $(MLFLAGS) -c $<

partialclean:
	rm -f *.o *.cm*

clean: partialclean
	rm -f *.a
	rm -f mldepend.mk rtti.ml

distclean: clean

.PHONY: partialclean clean distclean

.PRECIOUS: Makefile
