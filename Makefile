CC := g++
SRCDIR := src
BUILDDIR := build

SRCEXT := cpp
TESTER := matrixTest.$(SRCEXT)
SOURCES := $(SRCDIR)/*/*.$(SRCEXT)
OBJECTS := $(patsubst $(SRCDIR)/*/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
LIB := -L lib
TARGET := $(BUILDDIR)/a.out

INCLUDE := include
SLE := include/csc450Lib_linalg_base include/csc450Lib_linalg_sle 
EIGEN := include/csc450Lib_linalg_eigensystems
LINALG := $(SLE) $(EIGEN)
CALC := include/csc450Lib_calc_base include/csc450Lib_calc_snle

INC := $(LINALG) $(CALC)  $(INCLUDE)
INC_PARAMS=$(foreach d, $(INC), -I $d)

all: $(SOURCES)
	$(CC) $(INC_PARAMS) $(LIB) $^ $(TESTER) -o $(TARGET)

clean:
	rm $(TARGET)
