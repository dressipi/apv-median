video:
	rm -f video.mp4
	ffmpeg \
	-r 25 \
	-i tmp/%08d.png \
	-c:v libx264 \
	-vf fps=25 \
	-pix_fmt yuv420p video.mp4

png: make-png
	./make-png

clean-json:
	find tmp -name '*.json' | xargs -r rm

clean-png:
	find tmp -name '*.png' | xargs -r rm

clean: clean-json clean-png

.PHONY: plots json count-json clean-json clean
