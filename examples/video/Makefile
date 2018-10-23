all:
	echo 'run "make png video"'

video: make-mp4
	./make-mp4 video.mp4

png: make-png bin/dumper
	./make-png

bin/dumper:
	$(MAKE) -C dumper dumper

clean-json:
	find tmp -name '*.json' | xargs -r rm

clean-png:
	find tmp -name '*.png' | xargs -r rm

clean-dumper:
	$(MAKE) -C dumper clean

clean: clean-json clean-png clean-dumper

.PHONY: plots json count-json clean-json clean
