LIB = libapvmedian.a
OBJ = histogram.o median.o node.o
HDR = bin.h config.h histogram.h median.h node.h

default : all

include ../Common.mk

CFLAGS +=
LDLIBS += -lm

headerdir = $(includedir)/apv-median

all: $(LIB)

$(LIB): $(OBJ)
	ar rs $(LIB) $(OBJ)

install: install-lib install-headers

install-lib: $(LIB)
	$(INSTALL) -d $(libdir)
	$(INSTALL) -m 644 $(LIB) $(libdir)

install-headers: $(HDR)
	$(INSTALL) -d $(headerdir)
	$(INSTALL) -m 644 $(HDR) $(headerdir)

uninstall: uninstall-lib uninstall-headers

uninstall-lib:
	$(RM) $(libdir)/$(LIB)

uninstall-headers:
	for hdr in $(HDR) ; do $(RM) $(headerdir)/$$hdr ; done

clean:
	$(RM) $(LIB) $(OBJ)

spotless: clean

.PHONY: default all clean
