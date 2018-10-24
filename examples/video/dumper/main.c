#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>

#include <histogram.h>

#include "options.h"

#define PATH_LEN 128

int main(int argc, char** argv)
{
  struct gengetopt_args_info info;

  if (options(argc, argv, &info) != 0)
    {
      fprintf(stderr,"failed to parse command line\n");
      return EXIT_FAILURE;
    }

  histogram_t *hist;

  if ((hist = histogram_new(info.bins_arg)) == NULL)
    {
      fprintf(stderr,"failed to initialise histogram\n");
      return EXIT_FAILURE;
    }

  double capacity = info.capacity_given ? info.capacity_arg : DBL_MAX;
  size_t count = 0;
  double value;
  char path[PATH_LEN];

  while (scanf("%lf", &value) != EOF)
    {
      if (histogram_add(hist, value) != 0)
	{
	  fprintf(stderr, "error adding %f to histogram\n", value);
	  return EXIT_FAILURE;
	}

      if (histogram_capacity(hist, capacity) != 0)
        {
          fprintf(stderr, "failed capacity for %f\n", capacity);
          return 1;
        }

      snprintf(path, PATH_LEN, "%s/%08zi.json", info.directory_arg, count);

      if (histogram_json_save(hist, path) != 0)
	{
	  fprintf(stderr, "error saving histogram to %s\n", path);
	  return EXIT_FAILURE;
	}

      count++;
    }

  return EXIT_SUCCESS;
}
