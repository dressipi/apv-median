#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <histogram.h>
#include <median.h>

#include "options.h"

static int process(histogram_t *hist, FILE *stream)
{
  size_t count = 0;
  double value;

  while (scanf("%lf", &value) != EOF)
    {
      if (histogram_add(hist, value) != 0)
	{
	  fprintf(stderr, "error adding %f to histogram\n", value);
	  return 1;
	}

      double m;

      if (median(hist, &m) != 0)
	{
	  fprintf(stderr, "error getting mediann");
	  return 1;
	}

      fprintf(stream, "%li %e\n", count, m);
      count++;
    }

  return 0;
}

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

  int err;

  if (info.output_given)
    {
      FILE *stream;
      if ((stream = fopen(info.output_arg, "w")) == NULL)
	{
	  fprintf(stderr, "fialed to open %s\n", info.output_arg);
	  return EXIT_FAILURE;
	}
      err = process(hist, stream);
      fclose(stream);
    }
  else
    {
      err = process(hist, stdout);
    }

  return (err ? EXIT_FAILURE : EXIT_SUCCESS);
}
