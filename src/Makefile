LIB = libapvmedian.a
OBJ = avltree.o

default : all

# include Common.mk

CFLAGS += -Wall -std=c99 -O
LDLIBS += -lm

all: $(LIB)

$(LIB): $(OBJ)
	ar rs $(LIB) $(OBJ)

clean:
	$(RM) $(LIB) $(OBJ)

spotless: clean

.PHONY: default all clean
