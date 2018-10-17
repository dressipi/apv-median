# Makefile for CUnit test-suite

OBJ = main.o tests.o tests_avltree.o tests_histogram.o tests_median.o
RUBBISH += $(OBJ) suite *~
CFLAGS += -I../src
LDLIBS += -lapvmedian -lm
LDFLAGS += -L../src

default: run

include ../Common.mk

run: suite
	./suite -v

suite: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o suite

clean spotless:
	$(RM) $(RUBBISH)
