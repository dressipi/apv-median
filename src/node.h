/*
  node.h
  Copyright (c) Dressipi 2018
*/

#ifndef NODE_H
#define NODE_H

#include "bin.h"

typedef struct node_t node_t;

struct node_t
{
  bin_t bin;
  node_t *next;
};

extern node_t* node_new(double, node_t*);
extern void node_destroy(node_t*);

#endif
