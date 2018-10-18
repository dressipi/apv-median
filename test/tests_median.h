/*
  tests_median.h
  Copyright (c) Dressipi 2018
*/

#ifndef TESTS_MEDIAN_H
#define TESTS_MEDIAN_H

#include <CUnit/CUnit.h>

extern CU_TestInfo tests_median[];

extern void test_median_small_permutations(void);
extern void test_median_small_equal(void);
extern void test_median_large_equal(void);
extern void test_median_uniform(void);
extern void test_median_half_gaussian(void);

#endif
