default: all

include Common.mk

SUBDIRS = src test util
RUBBISH = *~ tmp/*.log
CONFFILES = Common.mk config.cache config.log config.status

all:
	make -C src all
	make -C util all

install:
	make -C src install
	make -C util install

uninstall:
	make -C src uninstall
	make -C util uninstall


ifeq ($(strip $(WITH_UNIT)),1)

test: install
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
	rm -rf tmp/

spotless-subdirs:
	for subdir in $(SUBDIRS) ; do $(MAKE) -C $$subdir spotless ; done

.PHONY: test all clean spotless
