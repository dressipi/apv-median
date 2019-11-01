/*
  tests_percentile.h
  Copyright (c) Stylemania Ltd. 2018
*/

#ifndef TESTS_PERCENTILE_H
#define TESTS_PERCENTILE_H

#include <CUnit/CUnit.h>

extern CU_TestInfo tests_percentile[];

extern void test_percentile_small_permutations(void);
extern void test_percentile_uniform(void);
extern void test_percentile_half_gaussian(void);
extern void test_percentile_non_decreasing(void);
extern void test_percentile_single_bin(void);

#endif
