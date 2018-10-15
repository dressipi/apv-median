/*
  median.c
  Copyright (c) Dressipi 2018
*/

#include <string.h>

#include "median.h"
#include "avltree.h"

struct bin_t
{
  size_t count;
  double max;
};

struct histogram_t
{
  size_t n;
  avltree_t *tree;
};


static int bin_cmp(const void *b1, const void *b2)
{
  double
    max1 = ((bin_t*)b1)->max,
    max2 = ((bin_t*)b2)->max;

  if (max1 < max2) return -1;
  if (max1 > max2) return 1;
  return 0;
}

static void* bin_dup(void *b)
{
  bin_t *bdup;

  if ((bdup = malloc(sizeof(bin_t))) == NULL)
    return NULL;

  return memcpy(bdup, b, sizeof(bin_t));
}

extern histogram_t* new_histogram(size_t n)
{
  histogram_t *hist;

  if ((hist = malloc(sizeof(histogram_t))) != NULL)
    {
      avltree_t *tree;

      if ((tree = avlnew(bin_cmp, bin_dup, free)) != NULL)
	{
	  hist->n = n;
	  hist->tree = tree;

	  return hist;
	}

      free(hist);
    }

  return NULL;
}
