/*
  assert_errno.h
  Copyright (c) Dressipi 2018
*/

#ifndef ASSERT_ERRNO_H
#define ASSERT_ERRNO_H

#include <errno.h>

#include <CUnit/CUnit.h>

extern void clear_errno(void);
extern void assert_errno(int);

#define CU_CLEAR_ERRNO() clear_errno()
#define CU_ASSERT_ERRNO(val) assert_errno(val)

#endif
