apv-median
----------

This an experimental library for estimating medians from large
datasets in a streaming fashion.  The algorithm is as described
in "Two Maximum Entropy Based Algorithms for Running Quantile
Estimation in Non-Stationary Data Streams", Ognjen Arandjelović,
Duc-Son Pham, Svetha Venkatesh,  IEEE Trans. Circ. Sys. Video
Tech., 25 (9), Sep. 2015
[DOI](https://doi.org/10.1109/TCSVT.2014.2376137).

The algorithm uses insert/delete operations on an ordered data
structure, so this repository includes am AVL-tree implementation;
originally by Julienne Walker (the source can be found at
[here](http://www.eternallyconfuzzled.com/libs/jsw_avltree.zip))
but modified to include unit tests and more-recent features of
the C language.

[![Build Status](https://ci.dressipi.com:3001/job/apv-median/badge/icon)](https://ci.dressipi.com:3001/job/apv-median/)
