#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>

#include <histogram.h>

#include "options.h"

#define PATH_LEN 128

int main(int argc, char **argv)
{
  struct gengetopt_args_info info;

  if (options(argc, argv, &info) != 0)
    {
      fprintf(stderr,"failed to parse command line\n");
      return EXIT_FAILURE;
    }

  if (info.inputs_num != 1)
    {
      fprintf(stderr, "path to histogram required\n");
      return EXIT_FAILURE;
    }

  const char *path = info.inputs[0];
  histogram_t *hist;

  if ((hist = histogram_json_load(path)) == NULL)
    {
      fprintf(stderr, "failed to read histogram from %s\n", path);
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
