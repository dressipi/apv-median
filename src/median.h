/*
  median.h
  Copyright (c) Dressipi 2018
*/

#ifndef MEDIAN_H
#define MEDIAN_H

#include <stdlib.h>

typedef struct bin_t bin_t;
typedef struct histogram_t histogram_t;

extern histogram_t* histogram_new(size_t);
extern void histogram_destroy(histogram_t*);

#endif
