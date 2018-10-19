/*
  histogram.h
  Copyright (c) Dressipi 2018
*/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdlib.h>
#include <stdio.h>

#include "bin.h"

typedef struct histogram_t histogram_t;

extern histogram_t* histogram_new(size_t);
extern void histogram_destroy(histogram_t*);
extern size_t histogram_num_bins(const histogram_t*);
extern bin_t* histogram_bins(const histogram_t*);
extern int histogram_add(histogram_t*, double);
extern histogram_t* histogram_json_load(const char*);
extern histogram_t* histogram_json_load_stream(FILE*);
extern int histogram_json_save(const histogram_t*, const char*);
extern int histogram_json_save_stream(const histogram_t*, FILE*);

#endif
