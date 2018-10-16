/*
  tests_median.c
  Copyright (c) Dressipi 2018
*/

#include <median.h>

#include "tests_median.h"

CU_TestInfo tests_median[] =
  {
    {"histogram_new non-NULL for positive n", test_median_new_non_null},
    {"histogram_new NULL for zero n", test_median_new_zero},
    {"histogram_add distinct returns zero", test_median_add_distinct},
    {"histogram_add equal returns zero", test_median_add_equal},
    {"histogram_add post-initialise", test_median_post_init},
    {"median small permutations", test_median_small_permutations},
    CU_TEST_INFO_NULL,
  };

extern void test_median_new_non_null(void)
{
  histogram_t *hist = histogram_new(5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);
  histogram_destroy(hist);
}

extern void test_median_new_zero(void)
{
  histogram_t *hist = histogram_new(0);
  CU_ASSERT_PTR_NULL(hist);
}

extern void test_median_add_distinct(void)
{
  histogram_t *hist = histogram_new(5);

  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < 10 ; i++)
    {
      int res = histogram_add(hist, i);
      CU_ASSERT_EQUAL(res, 0);
    }

  histogram_destroy(hist);
}

extern void test_median_add_equal(void)
{
  histogram_t *hist = histogram_new(5);

  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < 10 ; i++)
    {
      int res = histogram_add(hist, 6);
      CU_ASSERT_EQUAL(res, 0);
    }

  histogram_destroy(hist);
}

extern void test_median_post_init(void)
{
  histogram_t *hist = histogram_new(5);

  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < 10 ; i++)
    {
      int res = histogram_add(hist, i);
      CU_ASSERT_EQUAL_FATAL(res, 0);
    }

  int res = histogram_add(hist, 5.5);

  CU_ASSERT_EQUAL(res, 0);

  histogram_destroy(hist);
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
