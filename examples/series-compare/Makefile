all: series.pdf

CSV = approx.csv exact.csv

$(CSV): make-csv dumper/dumper
	./make-csv

series.pdf: make-pdf $(CSV)
	./make-pdf

dumper/dumper:
	$(MAKE) -C dumper dumper

clean:
	$(RM) $(CSV) series.ps gmt.history
	$(MAKE) -C dumper clean

.PHONY: all clean
