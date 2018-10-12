/*
  tests_avltree.h
  Copyright (c) Dressip 2018
*/

#ifndef TESTS_AVLTREE_H
#define TESTS_AVLTREE_H

#include <CUnit/CUnit.h>

extern CU_TestInfo tests_avltree[];

extern void test_avltree_new_not_null(void);
extern void test_avltree_new_empty(void);
extern void test_avltree_insert_count(void);
extern void test_avltree_find_present(void);
extern void test_avltree_find_absent(void);
extern void test_avltree_erase_present(void);
extern void test_avltree_erase_absent(void);

#endif
