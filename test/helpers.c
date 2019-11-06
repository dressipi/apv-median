/*
  helpers.c
  Copyright (c) Stylemania 2019
*/

#include <math.h>
#include <string.h>

#include "helpers.h"

extern int dbl_cmp(const void *a, const void *b)
{
  const double *da = a, *db = b;
  return (*da > *db) - (*da < *db);
}

extern double percentile_exact(size_t n, const double *v, double percent)
{
  double w[n];

  memcpy(w, v, n * sizeof(double));
  qsort(w, n, sizeof(double), dbl_cmp);

  if (percent == 100.0)
    return w[n-1];

  double p = n * (percent / 100.0);
  size_t m = floor(p);
  double f = p - m;

  if (f > 0)
    return w[m] * (1 - f) + w[m + 1] * f;
  else
    return w[m];
}

extern double median_exact(size_t n, const double *v)
{
  double w[n];

  memcpy(w, v, n * sizeof(double));
  qsort(w, n, sizeof(double), dbl_cmp);
  if (n % 2)
    return w[n/2];
  else
    return (w[n/2 - 1] + w[n/2]) / 2;
}

extern double rand_uniform(void)
{
  return (double)rand() / RAND_MAX;
}

/* this is Box-Muller */

extern double rand_half_gaussian(void)
{
  double
    u1 = rand_uniform(),
    u2 = rand_uniform(),
    R = sqrt(-log(u1)),
    t = M_PI * u2;

  return R * sin(t);
}
