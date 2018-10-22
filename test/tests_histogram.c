/*
  tests_histogram.c
  Copyright (c) Dressipi 2018
*/

#include <histogram.h>
#include <median.h>

#include "tests_histogram.h"

#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>

CU_TestInfo tests_histogram[] =
  {
    {"histogram_new non-NULL for positive n", test_histogram_new_non_null},
    {"histogram_new NULL for zero n", test_histogram_new_zero},
    {"histogram_add distinct returns zero", test_histogram_add_distinct},
    {"histogram_add equal returns zero", test_histogram_add_equal},
    {"histogram_add post-initialise", test_histogram_post_init},
    {"histogram_json_save", test_histogram_json_save},
    {"histogram_json_load", test_histogram_json_load},
    {"histogram JSON round-trip", test_histogram_json_roundtrip},
    CU_TEST_INFO_NULL,
  };

extern void test_histogram_new_non_null(void)
{
  histogram_t *hist = histogram_new(5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);
  histogram_destroy(hist);
}

extern void test_histogram_new_zero(void)
{
  histogram_t *hist = histogram_new(0);
  CU_ASSERT_PTR_NULL(hist);
}

extern void test_histogram_add_distinct(void)
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

extern void test_histogram_add_equal(void)
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

extern void test_histogram_post_init(void)
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

extern void test_histogram_json_save(void)
{
  histogram_t *hist = histogram_new(5);

  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < 10 ; i++)
    {
      int res = histogram_add(hist, i);
      CU_ASSERT_EQUAL_FATAL(res, 0);
    }

  double m;

  CU_ASSERT_EQUAL(median(hist, &m), 0);
  CU_ASSERT_DOUBLE_EQUAL(m, 4.0, 1e-6);

  const char *path = "tmp/histogram-save.json";
  struct stat stats;

  CU_ASSERT_EQUAL(histogram_json_save(hist, path), 0);
  CU_ASSERT_EQUAL(stat(path, &stats), 0);
  CU_ASSERT_EQUAL(unlink(path), 0);

  histogram_destroy(hist);
}

extern void test_histogram_json_load(void)
{
  const char *path = "fixtures/histogram.json";
  histogram_t *hist = histogram_json_load(path);
  double m;

  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  CU_ASSERT_EQUAL(histogram_num_bins(hist), 5);
  CU_ASSERT_EQUAL(median(hist, &m), 0);
  CU_ASSERT_DOUBLE_EQUAL(m, 4.0, 1e-6);

  histogram_destroy(hist);
}

extern void test_histogram_json_roundtrip(void)
{
  histogram_t *hist1 = histogram_new(20), *hist2;
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist1);

  for (size_t i = 1 ; i < 100 ; i++)
    {
      const char *path = "tmp/histogram-save.json";
      int res = histogram_add(hist1, i);
      CU_ASSERT_EQUAL(res, 0);

      double m1, m2;

      CU_ASSERT_EQUAL(median(hist1, &m1), 0);
      CU_ASSERT_EQUAL(histogram_json_save(hist1, path), 0);

      hist2 = histogram_json_load(path);

      CU_ASSERT_PTR_NOT_NULL_FATAL(hist2);
      CU_ASSERT_EQUAL(median(hist2, &m2), 0);

      CU_ASSERT_DOUBLE_EQUAL(m1, m2, 1e-6);

      CU_ASSERT_EQUAL(unlink(path), 0);
      histogram_destroy(hist2);
    }

  histogram_destroy(hist1);
}
