/*
  tests_histogram.h
  Copyright (c) Dressipi 2018
*/

#ifndef TESTS_HISTOGRAM_H
#define TESTS_HISTOGRAM_H

#include <CUnit/CUnit.h>

extern CU_TestInfo tests_histogram[];

extern void test_histogram_new_non_null(void);
extern void test_histogram_new_zero(void);
extern void test_histogram_add_distinct(void);
extern void test_histogram_add_equal(void);
extern void test_histogram_post_init(void);
extern void test_histogram_json_save(void);
extern void test_histogram_json_load(void);
extern void test_histogram_json_roundtrip(void);
extern void test_histogram_capacity_zero(void);
extern void test_histogram_capacity_empty(void);
extern void test_histogram_capacity_small(void);

#endif
