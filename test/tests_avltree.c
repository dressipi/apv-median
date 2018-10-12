/*
  tests_avltree.c

  Copyright (c) Dressipi 2018
*/

#include <avltree.h>

#include <stdlib.h>
#include <stdbool.h>

#include "tests_avltree.h"

CU_TestInfo tests_avltree[] =
  {
    {"new returns non-NULL", test_avltree_new_not_null},
    {"new has size zero", test_avltree_new_empty},
    {"insert modified count", test_avltree_insert_count},
    {"search for value present", test_avltree_find_present},
    {"search for value absent", test_avltree_find_absent},
    {"erase a value present", test_avltree_erase_present},
    {"erase a value absent", test_avltree_erase_absent},
    {"traverse find first", test_avltree_traverse_first},
    {"traverse find last", test_avltree_traverse_last},
    {"traverse find next", test_avltree_traverse_next},
    {"traverse find prev", test_avltree_traverse_prev},
    CU_TEST_INFO_NULL,
  };

static int cmp(const void *pn, const void *pm)
{
  int
    m = *(int*)pn,
    n = *(int*)pm;

  if (n < m)
    return 1;
  if (n > m)
    return -1;
  return 0;
}

static void* dup(void *pm)
{
  int *pn;

  if ((pn = malloc(sizeof(int))) == NULL)
    return NULL;

  *pn = *(int*)pm;

  return pn;
}

extern void test_avltree_new_not_null(void)
{
  avltree_t *tree = avlnew(cmp, dup, free);
  CU_ASSERT_PTR_NOT_NULL(tree);
  avldelete(tree);
}

extern void test_avltree_new_empty(void)
{
  avltree_t *tree = avlnew(cmp, dup, free);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tree);
  CU_ASSERT_EQUAL(avlsize(tree), 0);
  avldelete(tree);
}

extern void test_avltree_insert_count(void)
{
  avltree_t *tree = avlnew(cmp, dup, free);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tree);

  for (int i = 0 ; i < 10 ; i++)
    CU_ASSERT_EQUAL_FATAL(avlinsert(tree, &i), 1);

  CU_ASSERT_EQUAL(avlsize(tree), 10);
  avldelete(tree);
}

extern void test_avltree_find_present(void)
{
  avltree_t *tree = avlnew(cmp, dup, free);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tree);

  for (int i = 0 ; i < 10 ; i++)
    CU_ASSERT_EQUAL_FATAL(avlinsert(tree, &i), 1);

  int
    sought = 7,
    found = *(int*)avlfind(tree, &sought);

  CU_ASSERT_EQUAL(sought, found);
  avldelete(tree);
}

extern void test_avltree_find_absent(void)
{
  avltree_t *tree = avlnew(cmp, dup, free);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tree);

  for (int i = 0 ; i < 10 ; i++)
    CU_ASSERT_EQUAL_FATAL(avlinsert(tree, &i), 1);

  int sought = 15;

  CU_ASSERT_PTR_NULL(avlfind(tree, &sought));

  avldelete(tree);
}

extern void test_avltree_erase_present(void)
{
  avltree_t *tree = avlnew(cmp, dup, free);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tree);

  for (int i = 0 ; i < 10 ; i++)
    CU_ASSERT_EQUAL_FATAL(avlinsert(tree, &i), 1);

  int sought = 7;

  CU_ASSERT_PTR_NOT_NULL(avlfind(tree, &sought));
  CU_ASSERT_EQUAL(avlsize(tree), 10);
  CU_ASSERT_EQUAL(avlerase(tree, &sought), 1);
  CU_ASSERT_PTR_NULL(avlfind(tree, &sought));
  CU_ASSERT_EQUAL(avlsize(tree), 9);

  avldelete(tree);
}

extern void test_avltree_erase_absent(void)
{
  avltree_t *tree = avlnew(cmp, dup, free);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tree);

  for (int i = 0 ; i < 10 ; i++)
    CU_ASSERT_EQUAL_FATAL(avlinsert(tree, &i), 1);

  int sought = 15;

  CU_ASSERT_PTR_NULL(avlfind(tree, &sought));
  CU_ASSERT_EQUAL(avlsize(tree), 10);
  CU_ASSERT_EQUAL(avlerase(tree, &sought), 0);
  CU_ASSERT_PTR_NULL(avlfind(tree, &sought));
  CU_ASSERT_EQUAL(avlsize(tree), 10);

  avldelete(tree);
}

extern void test_avltree_traverse_first(void)
{
  avltree_t *tree = avlnew(cmp, dup, free);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tree);

  for (int i = 0 ; i < 10 ; i++)
    CU_ASSERT_EQUAL_FATAL(avlinsert(tree, &i), 1);

  avltrav_t *cursor = avltnew();
  CU_ASSERT_PTR_NOT_NULL_FATAL(cursor);

  int *found = avltfirst(cursor, tree);

  CU_ASSERT_PTR_NOT_NULL_FATAL(found);
  CU_ASSERT_EQUAL_FATAL(*found, 0);

  avltdelete(cursor);
  avldelete(tree);
}

extern void test_avltree_traverse_last(void)
{
  avltree_t *tree = avlnew(cmp, dup, free);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tree);

  for (int i = 0 ; i < 10 ; i++)
    CU_ASSERT_EQUAL_FATAL(avlinsert(tree, &i), 1);

  avltrav_t *cursor = avltnew();
  CU_ASSERT_PTR_NOT_NULL_FATAL(cursor);

  int *found = avltlast(cursor, tree);

  CU_ASSERT_PTR_NOT_NULL_FATAL(found);
  CU_ASSERT_EQUAL_FATAL(*found, 9);

  avltdelete(cursor);
  avldelete(tree);
}

extern void test_avltree_traverse_next(void)
{
  avltree_t *tree = avlnew(cmp, dup, free);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tree);

  for (int i = 0 ; i < 10 ; i++)
    CU_ASSERT_EQUAL_FATAL(avlinsert(tree, &i), 1);

  avltrav_t *cursor = avltnew();
  CU_ASSERT_PTR_NOT_NULL_FATAL(cursor);

  int *found = avltfirst(cursor, tree);

  CU_ASSERT_PTR_NOT_NULL_FATAL(found);
  CU_ASSERT_EQUAL_FATAL(*found, 0);

  int expected = 1;

  do
    {
      found = avltnext(cursor);
      if (found == NULL) break;
      CU_ASSERT_EQUAL(*found, expected++);
    }
  while (true);

  CU_ASSERT_EQUAL(expected, 10);

  avltdelete(cursor);
  avldelete(tree);
}

extern void test_avltree_traverse_prev(void)
{
  avltree_t *tree = avlnew(cmp, dup, free);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tree);

  for (int i = 0 ; i < 10 ; i++)
    CU_ASSERT_EQUAL_FATAL(avlinsert(tree, &i), 1);

  avltrav_t *cursor = avltnew();
  CU_ASSERT_PTR_NOT_NULL_FATAL(cursor);

  int *found = avltlast(cursor, tree);

  CU_ASSERT_PTR_NOT_NULL_FATAL(found);
  CU_ASSERT_EQUAL_FATAL(*found, 9);

  int expected = 8;

  do
    {
      found = avltprev(cursor);
      if (found == NULL) break;
      CU_ASSERT_EQUAL(*found, expected--);
    }
  while (true);

  CU_ASSERT_EQUAL(expected, -1);

  avltdelete(cursor);
  avldelete(tree);
}
