video: make-mp4
	./make-mp4 video.mp4

png: make-png
	./make-png

clean-json:
	find tmp -name '*.json' | xargs -r rm

clean-png:
	find tmp -name '*.png' | xargs -r rm

clean: clean-json clean-png

.PHONY: plots json count-json clean-json clean
