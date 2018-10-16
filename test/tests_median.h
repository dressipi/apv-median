/*
  tests_median.h
  Copyright (c) Dressipi 2018
*/

#ifndef TESTS_MEDIAN_H
#define TESTS_MEDIAN_H

#include <CUnit/CUnit.h>

extern CU_TestInfo tests_median[];

extern void test_median_new_non_null(void);
extern void test_median_new_zero(void);
extern void test_median_add_distinct(void);
extern void test_median_add_equal(void);
extern void test_median_post_init(void);
extern void test_median_small_permutations(void);

#endif
