/*
  cunit_compat.h

  Compatibility macros to handle different versions of CUnit,
  in particular the changes to the suite struct introduced in
  version 2.1-3

  Copyright (c) J.J. Green 2016
  This code is licensed under MIT, enjoy
*/

#ifndef CUNIT_COMPAT_H
#define CUNIT_COMPAT_H

#ifdef HAVE_CONFIG_H
#include <apv-median/config.h>
#endif

/* default to 2.1-2 or earlier (for now) */

#ifndef CUNIT_213
#define CUNIT_213 0
#endif

#if CUNIT_213
#define CU_Suite_Entry(a, b, c, d) {(a), (b), (c), NULL, NULL, (d)}
#else
#define CU_Suite_Entry(a, b, c, d) {(a), (b), (c), (d)}
#endif

#endif
