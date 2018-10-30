LIB = libapvmedian.a
OBJ = histogram.o median.o node.o
HDR = bin.h config.h histogram.h median.h node.h

default : all

include ../Common.mk

CFLAGS +=
LDLIBS += -lm

all: $(LIB)

$(LIB): $(OBJ)
	ar rs $(LIB) $(OBJ)

install: install-lib install-headers

install-lib: $(LIB)
	$(INSTALL) -d $(libdir)
	$(INSTALL) -m 644 $(LIB) $(libdir)

install-headers: $(HDR)
	$(INSTALL) -d $(includedir)
	$(INSTALL) -m 644 $(HDR) $(includedir)

uninstall: uninstall-lib uninstall-headers

uninstall-lib:
	$(RM) $(libdir)/$(LIB)

uninstall-headers:
	for hdr in $(HDR) ; do $(RM) $(includedir)/$$hdr ; done

clean:
	$(RM) $(LIB) $(OBJ)

spotless: clean

.PHONY: default all clean
