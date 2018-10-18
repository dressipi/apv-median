/*
  histogram.c
  Copyright (c) Dressipi 2018
*/

#include <math.h>
#include <string.h>

#include "histogram.h"

struct histogram_t
{
  size_t n, k;
  bin_t *bins;
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

/*
  this isn't the entropy, but in the context used is proportional
  to it, which is enough for our purposes
*/

static double entropy(double c)
{
  return -c * log(c);
}

/*
  merge a pair of adjacent bins in such a way as to maximise
  the entropy of the set of bins.  On entry, the histogram
  has n+1 bins (a new one is added in histogra_add()), on exit
  this will be reduced to n bins.
*/

static int maxent(histogram_t *hist)
{
  bin_t *bins = hist->bins;
  size_t n = hist->n, jmax = 0;
  double dEmax = -INFINITY;

  /*
    Find the pair (at offsets jmax, jmax+1) whose merge
    will maximise the total entropy, we inspect each pair
    and calculate the change in entropy (dE) resulting
    from the merge, then just choose the largest
  */

  for (size_t i = 0 ; i < n ; i++)
    {
      double
	c1 = bins[i].count,
	c2 = bins[i+1].count,
	dE = entropy(c1 + c2) - entropy(c1) - entropy(c2);

      if (dE > dEmax)
	{
	  jmax = i;
	  dEmax = dE;
	}
    }

  bins[jmax].count += bins[jmax+1].count;
  bins[jmax].max = bins[jmax+1].max;
  memmove(bins + jmax + 1,
	  bins + jmax + 2,
	  (n - jmax - 1) * sizeof(bin_t));

  return 0;
}


extern size_t histogram_num_bins(const histogram_t *hist)
{
  return hist->k;
}


extern bin_t* histogram_bins(const histogram_t *hist)
{
  return hist->bins;
}


extern int histogram_add(histogram_t *hist, double t)
{
  bin_t *bins = hist->bins;
  size_t k = hist->k, n = hist->n;

  /*
    The initialization of the histogram in the proposed method can be
    achieved in the same manner as in the interpolated bins algorithm
    introduced previously. To repeat, until the buffer is filled, i.e.
    until the number of unique stream data points processed exceeds n,
    the maximal entropy histogram is constructed by making each unique
    data value the top boundary of a bin, thereby allocating each unique
    data value its own bin.
  */

  if (k < n)
    {
      for (size_t i = 0 ; i < k ; i++)
	{
	  if (bins[i].max >= t)
	    {
	      if (bins[i].max == t)
		{
		  bins[i].count += 1.0;
		}
	      else
		{
		  memmove(bins + i + 1, bins + i, (k - i) * sizeof(bin_t));
		  bins[i].max = t;
		  bins[i].count = 1.0;
		  hist->k++;
		}
	      return 0;
	    }
	}

      bins[k].max = t;
      bins[k].count = 1.0;
      hist->k++;

      return 0;
    }

  /* histogram now intialised */

  double min = 0.0;

  for (size_t i = 0 ; i < n ; i++)
    {
      if (bins[i].max >= t)
	{
	  double
	    c = bins[i].count,
	    max = bins[i].max,
	    alpha = (t - min) / (max - min);

	  bins[i].count = c * (1 - alpha);

	  memmove(bins + i + 1, bins + i, (n - i) * sizeof(bin_t));

	  bins[i].max = t;
	  bins[i].count = c * alpha + 1.0;

	  return maxent(hist);
	}

      min = bins[i].max;
    }

  bins[n].max = t;
  bins[n].count = 1.0;

  return maxent(hist);
}
