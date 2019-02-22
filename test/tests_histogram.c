/*
  tests_histogram.c
  Copyright (c) Stylemania Ltd. 2018
*/

#include <apv-median/histogram.h>
#include <apv-median/median.h>

#include "assert_errno.h"
#include "tests_histogram.h"

#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>


CU_TestInfo tests_histogram[] =
  {
    {"histogram_new for positive n", test_histogram_new_positive},
    {"histogram_new for zero n", test_histogram_new_zero},
    {"histogram_add distinct returns zero", test_histogram_add_distinct},
    {"histogram_add equal returns zero", test_histogram_add_equal},
    {"histogram_add post-initialise", test_histogram_post_init},
    {"histigram_add duplicates", test_histogram_add_duplicates},
    {"histogram_json_save", test_histogram_json_save},
    {"histogram_json_load", test_histogram_json_load},
    {"histogram JSON round-trip", test_histogram_json_roundtrip},
    {"histogram capacity zero", test_histogram_capacity_zero},
    {"histogram capacity empty", test_histogram_capacity_empty},
    {"histogram capacity small", test_histogram_capacity_small},
    CU_TEST_INFO_NULL,
  };

extern void test_histogram_new_positive(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);
  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  histogram_destroy(hist);
  CU_ASSERT_ERRNO(0);
}

extern void test_histogram_new_zero(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(0);
  CU_ASSERT_ERRNO(EINVAL);
  CU_ASSERT_PTR_NULL(hist);
}

extern void test_histogram_add_distinct(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < 10 ; i++)
    {
      int res = histogram_add(hist, i);
      CU_ASSERT_ERRNO(0);
      CU_ASSERT_EQUAL(res, 0);
    }

  histogram_destroy(hist);
  CU_ASSERT_ERRNO(0);
}

extern void test_histogram_add_equal(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < 10 ; i++)
    {
      int res = histogram_add(hist, 6);
      CU_ASSERT_ERRNO(0);
      CU_ASSERT_EQUAL(res, 0);
    }

  histogram_destroy(hist);
  CU_ASSERT_ERRNO(0);
}

extern void test_histogram_post_init(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < 10 ; i++)
    {
      int res = histogram_add(hist, i);
      CU_ASSERT_ERRNO(0);
      CU_ASSERT_EQUAL_FATAL(res, 0);
    }

  histogram_add(hist, 5.5);
  CU_ASSERT_ERRNO(0);

  histogram_destroy(hist);
  CU_ASSERT_ERRNO(0);
}

/*
  regression -- stress testing the session recommender (with
  realistic data) resulted in duplicates being added to the
  histogram post-initialise, this gave "zero width" bins and
  the entropy calculation returned NANs, meaning those binse
  were not merged, so persisted and eventually took over the
  entire histogram.  This test failed until the entropy()
  function handle zero (and close to zero) arguments in a
  sensible manner.
*/

extern void test_histogram_add_duplicates(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 1 ; i < 6 ; i++)
    {
      int res = histogram_add(hist, i);
      CU_ASSERT_ERRNO(0);
      CU_ASSERT_EQUAL_FATAL(res, 0);
    }

  for (size_t i = 1 ; i < 3 ; i++)
    {
      int res = histogram_add(hist, 2.5);
      CU_ASSERT_ERRNO(0);
      CU_ASSERT_EQUAL_FATAL(res, 0);
    }

  histogram_destroy(hist);
  CU_ASSERT_ERRNO(0);
}

extern void test_histogram_json_save(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < 10 ; i++)
    {
      int res = histogram_add(hist, i);
      CU_ASSERT_ERRNO(0);
      CU_ASSERT_EQUAL_FATAL(res, 0);
    }

  double m;

  CU_ASSERT_EQUAL(median(hist, &m), 0);
  CU_ASSERT_DOUBLE_EQUAL(m, 4.0, 1e-6);

  const char *path = "tmp/histogram-save.json";
  struct stat stats;

  CU_ASSERT_EQUAL(histogram_json_save(hist, path), 0);
  CU_ASSERT_ERRNO(0);

  CU_ASSERT_EQUAL(stat(path, &stats), 0);
  CU_ASSERT_ERRNO(0);

  CU_ASSERT_EQUAL(unlink(path), 0);
  CU_ASSERT_ERRNO(0);

  const char *badpath = "tmp/no/such/dir/file.json";

  CU_ASSERT_NOT_EQUAL(histogram_json_save(hist, badpath), 0);
  CU_ASSERT_ERRNO(ENOENT);

  CU_ASSERT_NOT_EQUAL(stat(badpath, &stats), 0);
  CU_ASSERT_ERRNO(ENOENT);

  histogram_destroy(hist);
  CU_ASSERT_ERRNO(0);
}

extern void test_histogram_json_load(void)
{
  CU_CLEAR_ERRNO();

  const char *path = "fixtures/histogram.json";
  histogram_t *hist = histogram_json_load(path);
  double m;

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  CU_ASSERT_EQUAL(histogram_num_bins(hist), 5);
  CU_ASSERT_ERRNO(0);

  CU_ASSERT_EQUAL(median(hist, &m), 0);
  CU_ASSERT_ERRNO(0);

  CU_ASSERT_DOUBLE_EQUAL(m, 4.0, 1e-6);

  histogram_destroy(hist);
  CU_ASSERT_ERRNO(0);
}

extern void test_histogram_json_roundtrip(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist1 = histogram_new(20), *hist2;

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist1);

  for (size_t i = 1 ; i < 100 ; i++)
    {
      const char *path = "tmp/histogram-save.json";
      int res = histogram_add(hist1, i);

      CU_ASSERT_ERRNO(0);
      CU_ASSERT_EQUAL(res, 0);

      double m1, m2;

      CU_ASSERT_EQUAL(median(hist1, &m1), 0);
      CU_ASSERT_ERRNO(0);

      CU_ASSERT_EQUAL(histogram_json_save(hist1, path), 0);
      CU_ASSERT_ERRNO(0);

      hist2 = histogram_json_load(path);

      CU_ASSERT_ERRNO(0);
      CU_ASSERT_PTR_NOT_NULL_FATAL(hist2);

      CU_ASSERT_EQUAL(median(hist2, &m2), 0);
      CU_ASSERT_ERRNO(0);

      CU_ASSERT_DOUBLE_EQUAL(m1, m2, 1e-6);

      CU_ASSERT_EQUAL(unlink(path), 0);
      CU_ASSERT_ERRNO(0);

      histogram_destroy(hist2);
      CU_ASSERT_ERRNO(0);
    }

  histogram_destroy(hist1);
  CU_ASSERT_ERRNO(0);
}

extern void test_histogram_capacity_zero(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  CU_ASSERT_NOT_EQUAL(histogram_capacity(hist, 0), 0);
  CU_ASSERT_ERRNO(EINVAL);

  histogram_destroy(hist);
  CU_ASSERT_ERRNO(0);
}

extern void test_histogram_capacity_empty(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  CU_ASSERT_EQUAL(histogram_capacity(hist, 1), 0);
  CU_ASSERT_ERRNO(0);

  histogram_destroy(hist);
  CU_ASSERT_ERRNO(0);
}

extern void test_histogram_capacity_small(void)
{
  CU_CLEAR_ERRNO();

  histogram_t *hist = histogram_new(5);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(hist);

  for (size_t i = 0 ; i < 10 ; i++)
    {
      int res = histogram_add(hist, i);
      CU_ASSERT_ERRNO(0);
      CU_ASSERT_EQUAL_FATAL(res, 0);
    }

  double m1, m2;

  CU_ASSERT_EQUAL(median(hist, &m1), 0);
  CU_ASSERT_ERRNO(0);

  CU_ASSERT_EQUAL(histogram_capacity(hist, 5), 0);
  CU_ASSERT_ERRNO(0);

  CU_ASSERT_EQUAL(median(hist, &m2), 0);
  CU_ASSERT_ERRNO(0);

  CU_ASSERT_DOUBLE_EQUAL(m1, m2, 1e-8);

  histogram_destroy(hist);
  CU_ASSERT_ERRNO(0);
}
