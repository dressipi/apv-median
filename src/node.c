/*
  node.c
  Copyright (c) J.J. Green 2018
*/

#include <stdlib.h>

#include "node.h"


extern node_t* node_new(double max, node_t *next)
{
  node_t *node;

  if ((node = malloc(sizeof(node_t))) == NULL)
    return NULL;

  node->bin.max = max;
  node->bin.count = 1.0;
  node->next = next;

  return node;
}


extern void node_destroy(node_t *node)
{
  if (node) node_destroy(node->next);
  free(node);
}
