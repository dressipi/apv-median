#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>

#include <CUnit/Basic.h>

#include "tests.h"

/*
  we don't want stderr during tests, but we make this switchable
  via macro for debugging during development //
*/

// #define NO_STDERR

int main(int argc, char** argv)
{
  bool verbose = false;
  int c;

  while ((c = getopt(argc, argv, "v")) != -1)
    {
      switch (c)
	{
	case 'v':
	  verbose = true;
	  break;
	default:
	  abort();
	}
    }

  CU_BasicRunMode mode = (verbose ? CU_BRM_VERBOSE : CU_BRM_SILENT);
  CU_ErrorAction error_action = CUEA_IGNORE;
  setvbuf(stdout, NULL, _IONBF, 0);

  if (CU_initialize_registry())
    {
      fprintf(stderr,"failed to initialise registry\n");
      return EXIT_FAILURE;
    }

  tests_load();
  CU_basic_set_mode(mode);
  CU_set_error_action(error_action);

  int status;

  /*
    suppress stderr during the tests, this is not
    that portable and we might need some autoconf
  */

#ifdef NO_STDERR

  int saved_stderr = dup(fileno(stderr));

  if (saved_stderr == -1)
    {
      printf("failed to duplicate stderr: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

  int proxy_stderr = open("/dev/null", O_RDWR|O_APPEND, 0600);

  if (proxy_stderr == -1)
    {
      printf("failed to open /dev/null: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

  if (dup2(proxy_stderr, fileno(stderr)) == -1)
    {
      printf("failed to redirect stderr: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

#endif

  status = CU_basic_run_tests();

#ifdef NO_STDERR

  fflush(stderr);
  close(proxy_stderr);

  if (dup2(saved_stderr, fileno(stderr)) == -1)
    {
      printf("failed to restore stderr: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

#endif

  int nfail = CU_get_number_of_failures();

  if (verbose)
    printf("\nSuite %s: %d failed\n",
	   (status == 0 ? "OK" : "errored"),
	   nfail);

  CU_cleanup_registry();

  return (nfail > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
