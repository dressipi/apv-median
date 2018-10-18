/*
  median.c
  Copyright (c) Dressipi 2018
*/

#include <string.h>
#include <math.h>
#include <stdio.h>

#include "median.h"

extern int median(const histogram_t *hist, double *value)
{
  size_t k = histogram_num_bins(hist);
  bin_t *bins = histogram_bins(hist);
  double total = 0.0;

  for (size_t i = 0 ; i < k ; i++)
    total += bins[i].count;

  double
    partial = 0.0,
    min = 0.0;

  for (size_t i = 0 ; i < k ; i++)
    {
      double
	max = bins[i].max,
	count = bins[i].count,
	x = min + ((total / 2) - partial) * (max - min) / count;

      if (x < max)
	{
	  *value = x;
	  return 0;
	}

      partial += count;
      min = max;
    }

  return 1;
}