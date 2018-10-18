/*
  tests_median.c
  Copyright (c) Dressipi 2018
*/

#include <median.h>
#include "tests_median.h"

#include <stdlib.h>

CU_TestInfo tests_median[] =
  {
    {"small permutations", test_median_small_permutations},
    {"small equal", test_median_small_equal},
    {"large equal", test_median_large_equal},
    {"uniform in [0, 1]", test_median_uniform},
    {"half-Gaussian", test_median_half_gaussian},
    CU_TEST_INFO_NULL,
  };

static int dbl_cmp(const void *a, const void *b)
{
  const double *da = a, *db = b;
  return (*da > *db) - (*da < *db);
}

static double median_exact(size_t n, const double *v)
{
  double w[n];

  memcpy(w, v, n * sizeof(double));
  qsort(w, n, sizeof(double), dbl_cmp);
  if (n % 2)
    return w[n/2];
  else
    return (w[n/2 - 1] + w[n/2]) / 2;
}

static void test_median_array(size_t n, const double *v, double x, double eps)
{
  histogram_t *hist = histogram_new(5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < n ; i++)
    {
      int res = histogram_add(hist, v[i]);
      CU_ASSERT_EQUAL(res, 0);
    }

  double m = 0.0;

  CU_ASSERT_EQUAL(median(hist, &m), 0);
  CU_ASSERT_NOT_EQUAL(m, 0);
  CU_ASSERT_DOUBLE_EQUAL(m, x, eps);
}

/*
  For the input data 1, 2, 3, the algorithm generated
  a CDF which is y = x, for x in [0, 3], and we have
  the 0.5-quartile at y = 3/2 which is x = 3/2 (while
  the median is 2, of course).

  This is a approximation algorithm, so bad appriximations
  at small counts and atypical distributions is expected,
  this test, rather, is to check for invariance under
  permutations
*/

extern void test_median_small_permutations(void)
{
  double v[6][3] = {
    {1, 2, 3},
    {1, 3, 2},
    {2, 1, 3},
    {2, 3, 1},
    {3, 1, 2},
    {3, 2, 1}
  };

  for (size_t i = 0 ; i < 6 ; i++)
    test_median_array(3, v[i], 1.5, 1e-6);
}

/*
  Equal value input are the worst case for this algorithm,
  since the retruned value will always be exactly half the
  true median; but a useful test to stress for edge-case
  behaviour.
*/

static void test_median_equal(size_t n)
{
  double v[n], t[3] = {1e-5, 1.0, 1e5};

  for (size_t i = 0 ; i < 3 ; i++)
    {
      for (size_t j = 0 ; j < n ; j++)
	v[j] = t[i];
      test_median_array(n, v, t[i] / 2, 1e-6);
    }
}

extern void test_median_small_equal(void)
{
  test_median_equal(4);
}

extern void test_median_large_equal(void)
{
  test_median_equal(40);
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

  return R * sin(t);
}

static void test_median_dist(double (*f)(void), double eps)
{
  size_t n = 1024;
  double v[n];

  srand(42);

  for (size_t i = 0 ; i < n ; i++) v[i] = f();

  double exact = median_exact(n, v);

  test_median_array(n, v, exact, eps);
}

/* uniform in [0, 1] */

extern void test_median_uniform(void)
{
  test_median_dist(rand_uniform, 5e-3);
}

/* half-Gaussian */

extern void test_median_half_gaussian(void)
{
  test_median_dist(rand_half_gaussian, 5e-3);
}
