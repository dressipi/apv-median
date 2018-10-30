Source notes
============

- The symlink `apv-median` is there so that adding this directory
  to the include path ensures that the files `apv-median/histogram.h`
  exists; this facility is used by tests, if you remove that
  symlink then you break the tests
