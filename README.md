apv-median
----------

This is a C99 library for estimating medians from large datasets
in a streaming fashion. See the [documentation](doc/README.md) for
further details.

Compilation of this code needs the [Jannson][1] library for the
serialisation and deserialisation of the histogram structure to JSON.

To compile, test and install

    ./configure --enable-tests
    make
    make test
    make install

![Build Status](https://ci.dressipi.com:3001/buildStatus/icon?job=apv-median)

[1]: http://www.digip.org/jansson/
