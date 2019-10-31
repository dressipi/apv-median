/*
  percentile.c
  Copyright (c) Stylemania Ltd. 2018, 2019
*/

#include <string.h>
#include <math.h>

#include "percentile.h"

extern int percentile(const histogram_t *hist, double percent, double *value)
{
  if ((percent < 0) || (percent > 100))
    return 1;

  size_t k = histogram_num_bins(hist);
  bin_t *bins = histogram_bins(hist);
  double total = 0.0, fraction = percent / 100.0;

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
	x = min + ((total * fraction) - partial) * (max - min) / count;

      if (x < max)
	{
	  *value = x;
	  return 0;
	}

      partial += count;
      min = max;
    }

  /*
    We should never reach here, since a percentile of any
    non-empty set of numbers should be between the min
    and max values (possibly equal to, in the degenerate
    case of equal values).
   */

  return 1;
}
