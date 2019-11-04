Testing in-place
----------------

We want our tests to access the source headers in the installed
fashion, i.e., with a

    #inlcude <apv-median/file.h>

but we also want to test without installing, hence in test we
append `-I./include` and have a symbolic link in this directory
to the source directory.
