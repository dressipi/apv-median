/*
  tests_avltree.c

  Copyright (c) Dressipi 2018
*/

#include <avltree.h>

#include "tests_avltree.h"

CU_TestInfo tests_avltree[] =
  {
    {"NULL", test_avltree_null},
    CU_TEST_INFO_NULL,
  };

extern void test_avltree_null(void)
{
  CU_ASSERT(1);
}
