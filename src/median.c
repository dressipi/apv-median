/*
  median.c
  Copyright (c) Dressipi 2018
*/

#include <string.h>
#include <math.h>

#include "median.h"

struct bin_t
{
  double max, count;
};

struct histogram_t
{
  size_t n, k;
  bin_t *bins;
  double total;
};


extern histogram_t* histogram_new(size_t n)
{
  histogram_t *hist;

  if (n == 0)
    return NULL;

  if ((hist = malloc(sizeof(histogram_t))) != NULL)
    {
      /* allocate for n + 1 bins */

      if ((hist->bins = malloc((n + 1)*sizeof(bin_t))) != NULL)
	{
	  hist->n = n;
	  hist->k = 0;

	  return hist;
	}
      else
	free(hist);
    }

  return NULL;
}


extern void histogram_destroy(histogram_t *hist)
{
  free(hist->bins);
  free(hist);
}

static int maxent(histogram_t *hist)
{
  bin_t *bins = hist->bins;
  size_t n = hist->n, jmax = 0;
  double dEmax = -INFINITY;

  /*
  for (size_t i = 0 ; i <= n ; i++)
    printf("| %f, %f\n", bins[i].max, bins[i].count);
  */

  for (size_t i = 0 ; i < n ; i++)
    {
      double
	c = bins[i].count + bins[i+1].count,
	dE = -c * log(c);

      if (dE > dEmax)
	{
	  jmax = i;
	  dEmax = dE;
	}

      //printf("%i %i %f %f\n", i, jmax, dEmax, dE);
    }

  //printf("<- %zi\n", jmax);

  bins[jmax].count += bins[jmax+1].count;
  bins[jmax].max = bins[jmax+1].max;
  memmove(bins + jmax + 1, bins + jmax + 2, (n - jmax - 1) * sizeof(bin_t));

  /*
  for (size_t i = 0 ; i < n ; i++)
    printf("| %f, %f\n", bins[i].max, bins[i].count);
  */

  return 0;
}

/*
  The initialization of the histogram in the proposed method can be
  achieved in the same manner as in the interpolated bins algorithm
  introduced previously. To repeat, until the buffer is filled, i.e.
  until the number of unique stream data points processed exceeds n,
  the maximal entropy histogram is constructed by making each unique
  data value the top boundary of a bin, thereby allocating each unique
  data value its own bin.
*/

extern int histogram_add(histogram_t *hist, double t)
{
  bin_t *bins = hist->bins;
  size_t k = hist->k, n = hist->n;

  //printf("-> %f\n", t);

  if (k < n)
    {
      /* histogram initialisation */
      /*
      printf("[%zi, %zi, %f] ", k, n, t);
      for (size_t i = 0 ; i < k ; i++)
	printf("%f ", bins[i].max);
      printf("\n");
      */
      for (size_t i = 0 ; i < k ; i++)
	{
	  if (bins[i].max >= t)
	    {
	      if (bins[i].max == t)
		{
		  bins[i].count += 1.0;

		  return 0;
		}
	      else
		{
		  memmove(bins + i + 1, bins + i, (k - i) * sizeof(bin_t));
		  bins[i].max = t;
		  bins[i].count = 1.0;
		  hist->k++;

		  return 0;
		}
	    }
	}

      bins[k].max = t;
      bins[k].count = 1.0;
      hist->k++;

      return 0;
    }

  /* histogram is intialised */

  double min = 0.0;

  for (size_t i = 0 ; i < n ; i++)
    {
      if (bins[i].max >= t)
	{
	  double
	    c = bins[i].count,
	    max = bins[i].max;

	  memmove(bins + i + 1, bins + i, (n - i) * sizeof(bin_t));
	  bins[i].max = t;
	  bins[i].count = c * (t - min) / (max - min) + 1.0;

	  return maxent(hist);
	}

      min = bins[i].max;
    }

  bins[n].max = t;
  bins[n].count = 1.0;

  return maxent(hist);
}
