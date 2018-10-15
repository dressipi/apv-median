/*
  median.c
  Copyright (c) Dressipi 2018
*/

#include <string.h>
#include <math.h>

#include "median.h"
#include "avltree.h"

struct bin_t
{
  bin_t *prev;
  double max, count;
};

struct histogram_t
{
  size_t n;
  double total;
  avltree_t *tree;
};


static int bin_cmp1(const void *b1, const void *b2)
{
  double
    max1 = ((bin_t*)b1)->max,
    max2 = ((bin_t*)b2)->max;

  if (max1 > max2) return 1;
  if (max1 < max2) return -1;
  return 0;
}

static int bin_cmp2(const void *b1, const void *b2)
{
  double
    max1 = ((bin_t*)b1)->max,
    max2 = ((bin_t*)b2)->max;

  printf("max1: %f, max2 %f\n", max1, max2);

  if (max1 >= max2)
    {
      /*
	an important implemention detail for avltree is that the
	comparison of node payload always uses the tree-node in
	the first argument; by construction this will have a valid
	or NULL prev pointer once we have completed initialisation.
      */

      bin_t *prev = ((bin_t*)b1)->prev;
      double min1 = (prev ? prev->max : -INFINITY);

      if (min1 < max2)
	return 0;
      else
	return 1;
    }
  else
    return -1;
}


static void* bin_dup(void *b)
{
  bin_t *bdup;

  if ((bdup = malloc(sizeof(bin_t))) == NULL)
    return NULL;

  return memcpy(bdup, b, sizeof(bin_t));
}


extern histogram_t* histogram_new(size_t n)
{
  histogram_t *hist;

  if (n == 0) return NULL;

  if ((hist = malloc(sizeof(histogram_t))) != NULL)
    {
      avltree_t *tree;

      if ((tree = avlnew(bin_cmp1, bin_dup, free)) != NULL)
	{
	  hist->n = n;
	  hist->total = 0;
	  hist->tree = tree;

	  return hist;
	}

      free(hist);
    }

  return NULL;
}


extern void histogram_destroy(histogram_t *hist)
{
  avldelete(hist->tree);
  free(hist);
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

static int histogram_add_init(histogram_t *hist, double t)
{
  bin_t
    b = { .count = 1, .max = t },
    *pb = avlfind(hist->tree, &b);

  if (pb == NULL)
    {
      if (! avlinsert(hist->tree, &b))
	return 1;
    }
  else
    {
      pb->count++;
    }

  hist->total++;

  return 0;
}

static void bin_print(void *bin, void *opt)
{
  bin_t *b = bin;
  FILE *st = opt;

  fprintf(st, "%f %p\n", b->max, b->prev);
}


static void histogram_print(histogram_t *hist)
{
  avliter(hist->tree, bin_print, stdout);
}


extern int histogram_add(histogram_t *hist, double t)
{
  size_t n = hist->n;

  if (avlsize(hist->tree) < n)
    {
      int err = histogram_add_init(hist, t);
      if (err)
	return err;

      if (avlsize(hist->tree) < n)
	return 0;

      /*
	replace the initial avltree by one which uses
	a different compare function, and which has valid
	previous pointers
      */

      avltree_t *tree;
      if ((tree = avlnew(bin_cmp2, bin_dup, free)) == NULL)
	return 1;

      avltrav_t *cursor;
      if ((cursor = avltnew()) == NULL)
	return 1;

      bin_t *prev = NULL;

      for (bin_t *bin = avltfirst(cursor, hist->tree) ;
	   bin ;
	   bin = avltnext(cursor))
	{
	  bin->prev = prev;
	  avlinsert(tree, bin);
	  prev = bin;
	}

      avltdelete(cursor);

      avldelete(hist->tree);
      hist->tree = tree;

      return 0;
    }

  /* post initialise */

  printf("search %f\n", t);

  bin_t
    target = { .max = t },
    *found = avlfind(hist->tree, &target);

  printf("done\n");

  if (found)
    {
      // FIXME
    }
  else
    {
      avltrav_t *cursor;
      if ((cursor = avltnew()) == NULL)
	return 1;

      bin_t *last = avltlast(cursor, hist->tree);

      target.prev = last;
      target.count = 1.0;

      printf("insert tail %f\n", t);

      if (! avlinsert(hist->tree, &target))
	return 1;

      printf("done\n");
    }

  histogram_print(hist);

  return 0;
}
