AC_INIT([libavpmedian],
	[0.2],
  	[jim.green@dressipi.com],
	[libavpmedian],
	[https://dressipi.com/])

dnl | defaults

opt_enable_tests=no

AC_ARG_ENABLE(tests,
        AC_HELP_STRING([--enable-tests],
	[unit testing support (default is no)]),
	[opt_enable_tests=$enableval])

dnl | require a C99 compiler, gcc preferred

AC_PROG_CC([gcc clang])
AC_PROG_CC_C99

dnl | gcc-specifics, note clang sets __GCC__ so these options
dnl | are active for clang

if eval "test $GCC"; then
   CFLAGS="$CFLAGS -Wall -Wextra"
   LDFLAGS="$LDFLAGS"
fi

dnl | configure setup

AC_PROG_MAKE_SET
AC_PROG_INSTALL

dnl | gengetopt C output is in this repo, so not needed for
dnl | compile, but will be needed to add extra CL options

AC_PATH_PROG(GGO, gengetopt)

dnl | headers

AC_HEADER_STDC

dnl | posix stuff which should be on reasonable machines

AC_CHECK_HEADER(getopt.h)
AC_CHECK_HEADER(unistd.h)

dnl | 3rd party headers

AC_CHECK_HEADERS(jansson.h, [], [AC_MSG_ERROR(jansson.h not found)])

dnl | libraries

AC_CHECK_LIB(jansson, json_pack, [], [AC_MSG_ERROR(libjansson not found)])

dnl | tests option

if test $opt_enable_tests = yes; then
   AC_CHECK_LIB(cunit, CU_add_test)
   AC_SUBST(WITH_UNIT, 1)

   AC_MSG_CHECKING([CUnit version])
   AC_COMPILE_IFELSE([
     AC_LANG_SOURCE([[
       #include <CUnit/CUnit.h>
       int main(void) {
         CU_SuiteInfo suite;
         suite.pSetUpFunc = NULL;
         return 0; }
       ]])
     ],
     [
       AC_MSG_RESULT([2.1-3 or later])
       cunit213=1
     ],
     [
       AC_MSG_RESULT([2.1-2 or earlier])
       cunit213=0
     ])
   AC_DEFINE_UNQUOTED(CUNIT_213,
     $cunit213,
     [Define if CUnit version 2.1-3 or later])
fi

dnl | output

AC_SUBST(INSTALL_PROGRAM)
AC_SUBST(INSTALL_DATA)

AC_DEFINE(VERSION, PACKAGE_VERSION, "version of the package")

AC_CONFIG_HEADER(src/config.h)
AC_OUTPUT([Common.mk])
