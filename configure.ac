AC_INIT([avpmedian],
	[0.1],
  	[jim.green@dressipi.com],
	[avpmedian],
	[https://dressipi.com/])

dnl | defaults

opt_enable_tests=no
opt_enable_coverage=no

AC_ARG_ENABLE(tests,
        AC_HELP_STRING([--enable-tests],
	[unit testing support (default is no)]),
	[opt_enable_tests=$enableval])

AC_ARG_ENABLE(coverage,
        AC_HELP_STRING([--enable-coverage],
	[code coverage (default is no)]),
        [opt_enable_coverage=$enableval])

dnl | require a C99 compiler, gcc preferred

AC_PROG_CC([gcc clang])
AC_PROG_CC_C99

dnl | gcc-specifics, note clang sets __GCC__ so these options
dnl | are active for clang

if eval "test $GCC"; then
   CFLAGS="$CFLAGS -Wall -Wextra -pthread"
   LDFLAGS="$LDFLAGS -pthread"
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

AC_CHECK_HEADER(fcntl.h)
AC_CHECK_HEADER(getopt.h)
AC_CHECK_HEADER(netdb.h)
AC_CHECK_HEADER(netinet/in.h)
AC_CHECK_HEADER(pthread.h)
AC_CHECK_HEADER(signal.h)
AC_CHECK_HEADER(sys/socket.h)
AC_CHECK_HEADER(sys/stat.h)
AC_CHECK_HEADER(sys/types.h)
AC_CHECK_HEADER(unistd.h)

dnl | library headers, may not be installed (in which case error)

AC_CHECK_HEADER(pthread.h, [], AC_MSG_ERROR(pthread.h not found))

AC_CHECK_LIB(pthread, pthread_create)

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

dnl | coverage option

if test $opt_enable_coverage = yes; then
   CFLAGS="$CFLAGS --coverage"
   LDFLAGS="$LDFLAGS --coverage"
fi

dnl | output

AC_SUBST(INSTALL_PROGRAM)
AC_SUBST(INSTALL_DATA)

AC_DEFINE(VERSION, PACKAGE_VERSION, "version of the package")

AC_CONFIG_HEADER(src/config.h)
AC_OUTPUT([Common.mk])
