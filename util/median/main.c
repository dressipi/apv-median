#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include <histogram.h>
#include <median.h>

#include "options.h"

static bool verbose = false;

static void info(const char *fmt, ...)
{
  if (verbose)
    {
      va_list ap;
      va_start(ap, fmt);
      vprintf(fmt, ap);
      va_end(ap);
    }
}

int main(int argc, char **argv)
{
  struct gengetopt_args_info opt;

  if (options(argc, argv, &opt) != 0)
    {
      fprintf(stderr,"failed to parse command line\n");
      return EXIT_FAILURE;
    }

  if (opt.inputs_num != 1)
    {
      fprintf(stderr, "path to histogram required\n");
      return EXIT_FAILURE;
    }

  verbose = opt.verbose_flag;

  const char *path = opt.inputs[0];

  info("This is median %s\n", OPTIONS_VERSION);

  histogram_t *hist;

  if ((hist = histogram_json_load(path)) == NULL)
    {
      fprintf(stderr, "failed to read histogram from %s\n", path);
      return EXIT_FAILURE;
    }

  info("read histogram from %s\n", path);

  double value;

  if (median(hist, &value) != 0)
    {
      fprintf(stderr, "error calculating median\n");
      return EXIT_FAILURE;
    }

  info("median value: "); printf("%g\n", value);

  info("done.\n");

  return EXIT_SUCCESS;
}
