# Makefile for CUnit test-suite

OBJ = main.o tests.o assert_errno.o helpers.o \
	tests_histogram.o \
	tests_median.o \
	tests_node.o \
	tests_percentile.o
RUBBISH += $(OBJ) suite *~
CFLAGS += -I./include
LDLIBS += -lapvmedian -lm
LDFLAGS += -L./lib

default: run

include ../Common.mk

run: suite
	./suite -v

memcheck: suite
	valgrind --leak-check=full ./suite -v

suite: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o suite

clean spotless:
	$(RM) $(RUBBISH)
