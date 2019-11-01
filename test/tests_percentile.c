/*
  tests_percentile.c
  Copyright (c) Stylemania Ltd. 2018
*/

#include <apv-median/percentile.h>

#include "assert_errno.h"
#include "tests_percentile.h"

#include <stdlib.h>

CU_TestInfo tests_percentile[] =
  {
    {"small permutations", test_percentile_small_permutations},
    {"uniform in [0, 1]", test_percentile_uniform},
    {"half-Gaussian", test_percentile_half_gaussian},
    {"non-decreasing", test_percentile_non_decreasing},
    {"single bin", test_percentile_single_bin},
    CU_TEST_INFO_NULL,
  };

/* helper functions */

static int dbl_cmp(const void *a, const void *b)
{
  const double *da = a, *db = b;
  return (*da > *db) - (*da < *db);
}

static double percentile_exact(size_t n, const double *v, double percent)
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

static void test_percentile_array(size_t n, const double *v, double percent,
                                  double x, double eps)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < n ; i++)
    {
      int res = histogram_add(hist, v[i]);
      CU_ASSERT_ERRNO(0);
      CU_ASSERT_EQUAL(res, 0);
    }

  double m = 0.0;

  CU_ASSERT_EQUAL(percentile(hist, percent, &m), 0);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_NOT_EQUAL(m, 0);
  CU_ASSERT_DOUBLE_EQUAL(m, x, eps);

  histogram_destroy(hist);
}


/*
  For the input data 1, 2, 3, the algorithm generated
  a CDF which is y = x, for x in [0, 3], and we have
  the 66.6th percentile at y = 2 which is x = 2
*/

extern void test_percentile_small_permutations(void)
{
  double v[6][3] = {
    {1, 2, 3},
    {1, 3, 2},
    {2, 1, 3},
    {2, 3, 1},
    {3, 1, 2},
    {3, 2, 1}
  };

  double p = 200.0 / 3;

  for (size_t i = 0 ; i < 6 ; i++)
    test_percentile_array(3, v[i], p, 2, 1e-6);
}

/* more realistic tests for random distibutions */

static double rand_uniform(void)
{
  return (double)rand() / RAND_MAX;
}

static double rand_half_gaussian(void)
{
  double
    u1 = rand_uniform(),
    u2 = rand_uniform(),
    R = sqrt(-log(u1)),
    t = M_PI * u2;

  /* this is Box-Muller */

  return R * sin(t);
}

static void test_percentile_dist(double (*f)(void), double percent, double eps)
{
  size_t n = 1024;
  double v[n];

  srand(42);

  for (size_t i = 0 ; i < n ; i++) v[i] = f();

  double exact = percentile_exact(n, v, percent);

  test_percentile_array(n, v, percent, exact, eps);
}


/* uniform in [0, 1] */

extern void test_percentile_uniform(void)
{
  test_percentile_dist(rand_uniform, 25, 1e-2);
  test_percentile_dist(rand_uniform, 50, 1e-2);
  test_percentile_dist(rand_uniform, 75, 1e-2);
}


/* half-Gaussian */

extern void test_percentile_half_gaussian(void)
{
  test_percentile_dist(rand_half_gaussian, 25, 5e-3);
  test_percentile_dist(rand_half_gaussian, 50, 5e-3);
  test_percentile_dist(rand_half_gaussian, 75, 4e-2);
}

/* non-decreasing with percentile */

extern void test_percentile_non_decreasing(void)
{
  size_t n = 1024;

  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < n ; i++)
    {
      int res = histogram_add(hist, rand_uniform());
      CU_ASSERT_ERRNO(0);
      CU_ASSERT_EQUAL(res, 0);
    }

  double last = 0.0;

  for (size_t i = 1 ; i < 100 ; i++)
    {
      double p;

      CU_ASSERT_EQUAL(percentile(hist, i, &p), 0);
      CU_ASSERT(p >= last);
      last = p;
    }

  histogram_destroy(hist);
}

extern void test_percentile_single_bin(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  int res = histogram_add(hist, 1.0);
  CU_ASSERT_ERRNO(0);
  CU_ASSERT_EQUAL(res, 0);

  for (int i = 0 ; i <= 100 ; i++)
    {
      double p;
      CU_ASSERT_EQUAL(percentile(hist, i, &p), 0);
      CU_ASSERT_DOUBLE_EQUAL(p, i / 100.0, 1e-8);
    }

  histogram_destroy(hist);
}
