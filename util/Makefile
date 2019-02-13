SUBDIRS = histogram-median

all: build

build:
	for dir in $(SUBDIRS) ; do make -C $$dir build ; done

install:
	for dir in $(SUBDIRS) ; do make -C $$dir install ; done

uninstall:
	for dir in $(SUBDIRS) ; do make -C $$dir uninstall ; done

clean:
	for dir in $(SUBDIRS) ; do make -C $$dir clean ; done

spotless:
	for dir in $(SUBDIRS) ; do make -C $$dir spotless ; done
