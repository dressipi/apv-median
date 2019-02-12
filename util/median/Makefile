NAME = median

default: $(NAME)

ROOT = ../..

OBJ = main.o options.o
RUBBISH += $(OBJ) $(NAME)
CFLAGS += -I$(ROOT)/src
LDFLAGS += -L$(ROOT)/src
LDLIBS := -lapvmedian -lm $(LDLIBS)

all: build install

include $(ROOT)/Common.mk

build: $(NAME)

install:
	$(INSTALL) -d $(bindir)
	$(INSTALL) -m755 $(NAME) $(bindir)/$(NAME)

clean:
	$(RM) $(NAME) $(OBJ)

spotless: clean

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(NAME)

%.h %.c: %.ggo
	$(GGO) -u -i $< -f $* -F $*

options.o: CFLAGS += -Wno-unused-but-set-variable
