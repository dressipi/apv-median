/*
  histogram.c
  Copyright (c) Dressipi 2018
*/

#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "histogram.h"

typedef struct node_t node_t;

struct node_t
{
  bin_t bin;
  node_t *next;
};

struct histogram_t
{
  size_t n, k;
  bin_t *bins;
  node_t *nodes;
};


static node_t* node_new(double max, node_t *next)
{
  node_t *node;

  if ((node = malloc(sizeof(node_t))) == NULL)
    return NULL;

  node->bin.max = max;
  node->bin.count = 1.0;
  node->next = next;

  return node;
}

static void node_destroy(node_t *node)
{
  if (node) node_destroy(node->next);
  free(node);
}

extern histogram_t* histogram_new(size_t n)
{
  histogram_t *hist;

  if (n == 0)
    return NULL;

  if ((hist = malloc(sizeof(histogram_t))) != NULL)
    {
      if ((hist->bins = malloc(n * sizeof(bin_t))) != NULL)
	{
	  hist->n = n;
	  hist->k = 0;
	  hist->nodes = NULL;

	  return hist;
	}
      else
	free(hist);
    }

  return NULL;
}

extern void histogram_destroy(histogram_t *hist)
{
  node_destroy(hist->nodes);
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

extern size_t histogram_num_bins(const histogram_t *hist)
{
  return hist->k;
}

extern bin_t* histogram_bins(const histogram_t *hist)
{
  bin_t *bins = hist->bins;
  node_t *node = hist->nodes;

  for (size_t i = 0 ; i < hist->k ; i++)
    {
      memcpy(bins + i, &(node->bin), sizeof(bin_t));
      node = node->next;
    }

  if (node != NULL) return NULL;

  return bins;
}

/*
  merge a pair of adjacent bins in such a way as to maximise
  the entropy of the set of bins.  On entry, the histogram
  has n+1 nodes (a new one is added in histogra_add()), on exit
  this will be reduced to n nodes.
*/

static int merge(histogram_t *hist)
{
  node_t *last = hist->nodes, *maxnode = last, *this;
  double dEmax = -INFINITY;

  while ((this = last->next) != NULL)
    {
      double
	c1 = last->bin.count,
	c2 = this->bin.count,
	dE = entropy(c1 + c2) - entropy(c1) - entropy(c2);

      if (dE > dEmax)
	{
	  maxnode = last;
	  dEmax = dE;
	}

      last = this;
    }

  node_t *next = maxnode->next;

  maxnode->bin.count += next->bin.count;
  maxnode->bin.max = next->bin.max;
  maxnode->next = next->next;

  free(next);

  return 0;
}


extern int histogram_add(histogram_t *hist, double t)
{
  size_t k = hist->k, n = hist->n;

  if (k == 0)
    {
      node_t *node;
      if ((node = node_new(t, NULL)) == NULL)
	return 1;
      hist->nodes = node;
      hist->k++;
    }
  else if (k < n)
    {
      /*
	The initialization of the histogram in the proposed method can be
	achieved in the same manner as in the interpolated bins algorithm
	introduced previously. To repeat, until the buffer is filled, i.e.
	until the number of unique stream data points processed exceeds n,
	the maximal entropy histogram is constructed by making each unique
	data value the top boundary of a bin, thereby allocating each unique
	data value its own bin.
      */

      if (t <= hist->nodes->bin.max)
	{
	  if (t == hist->nodes->bin.max)
	    hist->nodes->bin.count += 1.0;
	  else
	    {
	      node_t *node;
	      if ((node = node_new(t, hist->nodes)) == NULL)
		return 1;
	      hist->nodes = node;
	      hist->k++;
	    }
	}
      else
	{
	  node_t *last = hist->nodes;

	  do
	    {
	      node_t *this = last->next;

	      if (this == NULL)
		{
		  node_t *node;
		  if ((node = node_new(t, NULL)) == NULL)
		    return 1;
		  last->next = node;
		  hist->k++;
		  break;
		}

	      if (t <= this->bin.max)
		{
		  if (t == this->bin.max)
		    this->bin.count += 1.0;
		  else
		    {
		      node_t *node;
		      if ((node = node_new(t, this)) == NULL)
			return 1;
		      last->next = node;
		      hist->k++;
		    }
		  break;
		}

	      last = this;
	    }
	  while (true);
	}
    }
  else
    {
      /* post initialisation */

      if (t <= hist->nodes->bin.max)
	{
	  double
	    c = hist->nodes->bin.count,
	    max = hist->nodes->bin.max,
	    min = 0.0,
	    alpha = (t - min) / (max - min);
	  node_t *node;

	  if ((node = node_new(t, hist->nodes)) == NULL)
	    return 1;

	  node->bin.count = c * alpha + 1.0;
	  node->next->bin.count = c * (1 - alpha);

	  hist->nodes = node;
	}
      else
	{
	  node_t *last = hist->nodes;

	  do
	    {
	      double min = last->bin.max;
	      node_t *this = last->next;

	      if (this == NULL)
		{
		  node_t *node;
		  if ((node = node_new(t, NULL)) == NULL)
		    return 1;
		  last->next = node;
		  break;
		}

	      double max = this->bin.max;

	      if (t <= max)
		{
		  double
		    c = this->bin.count,
		    alpha = (t - min) / (max - min);

		  node_t *node;
		  if ((node = node_new(t, this)) == NULL)
		    return 1;

		  node->bin.count = c * alpha + 1.0;
		  node->next->bin.count = c * (1 - alpha);

		  last->next = node;
		  break;
		}

	      last = this;
	    }
	  while (true);
	}

      if (merge(hist) != 0)
	return 1;
    }

  return 0;
}
