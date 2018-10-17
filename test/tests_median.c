/*
  tests_median.c
  Copyright (c) Dressipi 2018
*/

#include <median.h>

#include "tests_median.h"

CU_TestInfo tests_median[] =
  {
    {"median small permutations", test_median_small_permutations},
    CU_TEST_INFO_NULL,
  };

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
