#include "assert_errno.h"

extern void clear_errno(void)
{
  errno = 0;
}

extern void assert_errno(int value)
{
  CU_ASSERT_EQUAL(errno, value);
  clear_errno();
}
