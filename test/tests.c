/*
  tests.c

  testcase loader

  Copyright (c) Dressipi 2018
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "tests_avltree.h"

#include "cunit_compat.h"

#define ENTRY(a, b) CU_Suite_Entry((a), NULL, NULL, (b))

#include <CUnit/CUnit.h>

static CU_SuiteInfo suites[] =
  {
    ENTRY("avltree", tests_avltree),
    CU_SUITE_INFO_NULL
  };

void tests_load(void)
{
  assert(NULL != CU_get_registry());
  assert(!CU_is_test_running());

  if (CU_register_suites(suites) != CUE_SUCCESS)
    {
      fprintf(stderr, "suite registration failed - %s\n",
	      CU_get_error_msg());
      exit(EXIT_FAILURE);
    }
}
