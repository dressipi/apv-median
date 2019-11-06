/*
  median.c
  Copyright (c) Stylemania Ltd. 2018
*/

#include "median.h"
#include "percentile.h"

extern int median(const histogram_t *hist, double *value)
{
  return percentile(hist, 50.0, value);
}
