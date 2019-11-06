/*
  helpers.h
  Copyright (c) Stylemania 2019
*/

#ifndef HELPERS_H
#define HELPERS_H

#include <stdlib.h>

extern int dbl_cmp(const void*, const void*);
extern double percentile_exact(size_t, const double*, double);
extern double median_exact(size_t, const double*);
extern double rand_uniform(void);
extern double rand_half_gaussian(void);

#endif
