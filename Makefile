default: all

include Common.mk

SUBDIRS = src test
RUBBISH = *~ tmp/*.log
CONFFILES = Common.mk config.cache config.log config.status

all:
	make -C src all

install:
	make -C src install


ifeq ($(strip $(WITH_UNIT)),1)

test: all
	make -C test run

else

test:
	echo 'tests not enabled, configure with --enable-tests'
	exit 1

endif

clean: clean-subdirs clean-cwd

clean-cwd:
	$(RM) $(RUBBISH)

clean-subdirs:
	for subdir in $(SUBDIRS) ; do $(MAKE) -C $$subdir clean ; done

spotless: spotless-subdirs spotless-cwd

spotless-cwd: clean-cwd
	$(RM) $(CONFFILES)
	autoconf
	rm -rf autom4te.cache/


spotless-subdirs:
	for subdir in $(SUBDIRS) ; do $(MAKE) -C $$subdir spotless ; done

.PHONY: test all clean spotless
