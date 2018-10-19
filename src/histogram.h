/*
  histogram.h
  Copyright (c) Dressipi 2018
*/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdlib.h>

#include "bin.h"

typedef struct histogram_t histogram_t;

extern histogram_t* histogram_new(size_t);
extern void histogram_destroy(histogram_t*);
extern size_t histogram_num_bins(const histogram_t*);
extern bin_t* histogram_bins(const histogram_t*);
extern int histogram_add(histogram_t*, double);

#endif
