/*
  tests_avltree.c

  Copyright (c) Dressipi 2018
*/

#include <avltree.h>

#include <stdlib.h>

#include "tests_avltree.h"

CU_TestInfo tests_avltree[] =
  {
    {"new returns non-NULL", test_avltree_new_not_null},
    {"new has size zero", test_avltree_new_empty},
    CU_TEST_INFO_NULL,
  };

typedef int (*cmp_f)(const void *p1, const void *p2);
typedef void* (*dup_f)(void *p);
typedef void (*rel_f)(void *p);

static int cmp_int(const int *n, const int *m)
{
  if (n < m)
    return 1;
  if (n > m)
    return -1;
  return 0;
}

static int* dup_int(int *m)
{
  int *p;

  if ((p = malloc(sizeof(int))) == NULL)
    return NULL;

  *p = *m;

  return p;
}

static void rel_int(int *m)
{
  free(m);
}

extern void test_avltree_new_not_null(void)
{
  avltree_t *tree = avlnew((cmp_f)cmp_int, (dup_f)dup_int, (rel_f)rel_int);
  CU_ASSERT_PTR_NOT_NULL(tree);
  avldelete(tree);
}

extern void test_avltree_new_empty(void)
{
  avltree_t *tree = avlnew((cmp_f)cmp_int, (dup_f)dup_int, (rel_f)rel_int);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tree);
  CU_ASSERT_EQUAL(avlsize(tree), 0);
  avldelete(tree);
}
