#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "options.h"

int main(int argc, char** argv)
{
  struct gengetopt_args_info info;

  if (options(argc, argv, &info) != 0)
    {
      fprintf(stderr,"failed to parse command line\n");
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
