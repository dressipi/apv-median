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
extern void test_median_add(void);

#endif
