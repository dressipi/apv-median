/*
  tests_node.c
  Copyright (c) Dressipi 2018
*/

#include <node.h>

#include "assert_errno.h"
#include "tests_node.h"

#include <stdlib.h>

CU_TestInfo tests_node[] =
  {
    {"constructor", test_node_new},
    CU_TEST_INFO_NULL,
  };

extern void test_node_new(void)
{
  CU_CLEAR_ERRNO();

  node_t *node = node_new(10, NULL);

  CU_ASSERT_ERRNO(0);
  CU_ASSERT_PTR_NOT_NULL_FATAL(node);

  CU_ASSERT_EQUAL(node->bin.max, 10.0);
  CU_ASSERT_EQUAL(node->bin.count, 1.0);
  CU_ASSERT_PTR_NULL(node->next);

  node_destroy(node);
  CU_ASSERT_ERRNO(0);
}
