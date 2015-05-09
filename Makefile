CC := g++
SRCDIR := src
BUILDDIR := build

SRCEXT := cpp
OBJECTS := $(SRCDIR)/*/*.$(SRCEXT)

INCLUDE := include
SLE := include/csc450Lib_linalg_base include/csc450Lib_linalg_sle 
EIGEN := include/csc450Lib_linalg_eigensystems
LINALG := $(SLE) $(EIGEN)
CALC := include/csc450Lib_calc_base include/csc450Lib_calc_snle

INC := $(LINALG) $(CALC)  $(INCLUDE)
INC_PARAMS=$(foreach d, $(INC), -I $d)

all: $(OBJECTS)
	$(CC) $(INC_PARAMS) $^ matrixTest.$(SRCEXT) -o $(BUILDDIR)/a.out

clean:
	rm build/*
	rm output/*.txt
