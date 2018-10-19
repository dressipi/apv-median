/*
  histogram.c
  Copyright (c) Dressipi 2018
*/

#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <jansson.h>

#include "histogram.h"
#include "node.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

struct histogram_t
{
  size_t n, k;
  bin_t *bins;
  node_t *nodes;
};


/* constructor / destructor */

extern histogram_t* histogram_new(size_t n)
{
  if (n == 0)
    return NULL;

  histogram_t *hist;

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
  save to and load from JSON
*/

extern histogram_t* histogram_json_load_stream(FILE *st)
{
  json_t *root;

  if ((root = json_loadf(st, 0, NULL)) == NULL)
    return NULL;

  json_t *array;

  if ((array = json_object_get(root, "nodes")) == NULL)
    return NULL;

  for (sizt_t i = 0; i < json_array_size(array); i++)
    {
      json_t *item;

      if ((item = json_array_get(root, i)) == NULL)
	return NULL;

      /* add item to hist */
    }

  return NULL;
}

extern histogram_t* histogram_json_load(const char *path)
{
  FILE *st;

  if ((st = fopen(path, "r")) == NULL)
    return NULL;

  histogram_t *hist = histogram_json_load_stream(st);

  if (fclose(st) == EOF)
    {
      if (hist != NULL) histogram_destroy(hist);
      return NULL;
    }

  return hist;
}

static int node_print_json(json_t *objs, node_t *node)
{
  json_t *obj = json_object();

  json_object_set_new(obj, "max", json_real(node->bin.max));
  json_object_set_new(obj, "count", json_real(node->bin.count));

  json_array_append(objs, obj);

  return 0;
}

static int nodes_print_json(json_t *objs, node_t *nodes)
{
  if (nodes == NULL)
    return 0;

  return
    node_print_json(objs, nodes) +
    nodes_print_json(objs, nodes->next);
}

static const char* date_string(void)
{
  time_t t = time(NULL);
  struct tm* bdt = gmtime(&t);
  static char buffer[32];

  snprintf(buffer, 32,
           "%04d-%02d-%02dT%02d:%02d",
           bdt->tm_year + 1900,
           bdt->tm_mon + 1,
           bdt->tm_mday,
           bdt->tm_hour,
           bdt->tm_min);

  return buffer;
}

extern int histogram_json_save_stream(const histogram_t* hist, FILE *st)
{
  json_t *objs;
  int err = 0;

  if ((objs = json_array()) != NULL)
    {
      if (nodes_print_json(objs, hist->nodes) == 0)
        {
          json_t *root;

          if ((root = json_object()) != NULL)
            {
	      json_t
		*version = json_string(VERSION),
		*created = json_string(date_string()),
		*creator = json_string(PACKAGE_NAME);

	      if (
		  (json_object_set_new(root, "creator", creator) == 0) &&
                  (json_object_set_new(root, "version", version) == 0) &&
                  (json_object_set_new(root, "created", created) == 0) &&
                  (json_object_set(root, "nodes", objs) == 0)
                  )
                {
                 if (json_dumpf(root, st, JSON_INDENT(2)) != 0)
		   err++;
		}
	      else
		err++;
	    }
	  else
	    err++;
	}
      else
	err++;
    }
  else
    err++;

  return err;
}

extern int histogram_json_save(const histogram_t* hist, const char *path)
{
  FILE *st;

  if ((st = fopen(path, "w")) == NULL)
    return 1;

  int err = histogram_json_save_stream(hist, st);

  if (fclose(st) == EOF)
    return 1;

  return err;
}


/*
  it is convenient for the median calculation to dump
  the histogram's bins to an array
*/

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

static double entropy(double);

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

  node_t *next;

  if ((next = maxnode->next) == NULL)
    return 1;

  maxnode->bin.count += next->bin.count;
  maxnode->bin.max = next->bin.max;
  maxnode->next = next->next;

  free(next);

  return 0;
}


/*
  this isn't the entropy, but in the context used is proportional
  to it, which is enough for our purposes
*/

static double entropy(double c)
{
  return -c * log(c);
}


/* add a bew value t to the histogram */

static int histogram_add_first(histogram_t*, double);
static int histogram_add_init(histogram_t*, double);
static int histogram_add_main(histogram_t*, double);

extern int histogram_add(histogram_t *hist, double t)
{
  size_t k = hist->k, n = hist->n;
  int (*f)(histogram_t*, double);

  if (k == 0)
    f = histogram_add_first;
  else if (k < n)
    f = histogram_add_init;
  else
    f = histogram_add_main;

  return f(hist, t);
}

static int histogram_add_first(histogram_t *hist, double t)
{
  node_t *node;

  if ((node = node_new(t, NULL)) == NULL)
    return 1;

  hist->nodes = node;
  hist->k++;

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

static int histogram_add_init(histogram_t *hist, double t)
{
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
	  node_t *this;

	  if ((this = last->next) == NULL)
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

  return 0;
}

static int histogram_add_main(histogram_t *hist, double t)
{
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
	  node_t *this;

	  if ((this = last->next) == NULL)
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

  return merge(hist);
}
