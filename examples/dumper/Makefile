default: dumper

ROOT = ../..

include $(ROOT)/Common.mk

OBJ = main.o options.o
RUBBISH += $(OBJ) dumper *~
CFLAGS += -I$(ROOT)/src
LDLIBS += -lapvmedian -ljansson -lm
LDFLAGS += -L$(ROOT)/src

dumper: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o dumper

%.h %.c : %.ggo
	gengetopt -u -i $< -f $* -F $*

options.o : CFLAGS += -Wno-unused-but-set-variable

clean:
	$(RM) $(RUBBISH)
