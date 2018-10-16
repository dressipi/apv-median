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
