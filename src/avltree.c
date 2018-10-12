/*
  AVL balanced tree library

    > Created (Julienne Walker): June 17, 2003
    > Modified (Julienne Walker): September 24, 2005
    > Modified (J.J. Green): 2018
*/

#include "avltree.h"

#include <stdlib.h>
#include <stdbool.h>

#ifndef HEIGHT_LIMIT
#define HEIGHT_LIMIT 64
#endif

typedef struct avlnode_t avlnode_t;

struct avlnode_t
{
  int balance;
  void *data;
  avlnode_t *link[2];
};

struct avltree_t
{
  avlnode_t *root;
  cmp_f cmp;
  dup_f dup;
  rel_f rel;
  size_t size;
};

struct avltrav_t
{
  avltree_t *tree;
  avlnode_t *it;
  avlnode_t *path[HEIGHT_LIMIT];
  size_t top;
};


static avlnode_t* rotate_single(avlnode_t *root, int dir)
{
  avlnode_t *save = root->link[!dir];

  root->link[!dir] = save->link[dir];
  save->link[dir] = root;

  return save;
}


static avlnode_t* rotate_double(avlnode_t *root, int dir)
{
  avlnode_t *save = root->link[!dir]->link[dir];

  root->link[!dir]->link[dir] = save->link[!dir];
  save->link[!dir] = root->link[!dir];
  root->link[!dir] = save;
  save = root->link[!dir];
  root->link[!dir] = save->link[dir];
  save->link[dir] = root;

  return save;
}


static void adjust_balance(avlnode_t *root, int dir, int bal)
{
  avlnode_t
    *n = root->link[dir],
    *nn = n->link[!dir];

  if (nn->balance == 0)
    {
      root->balance = n->balance = 0;
    }
  else if (nn->balance == bal)
    {
      root->balance = -bal;
      n->balance = 0;
    }
  else
    {
      root->balance = 0;
      n->balance = bal;
    }

  nn->balance = 0;
}


static avlnode_t* insert_balance(avlnode_t *root, int dir)
{
  avlnode_t *n = root->link[dir];
  int bal = (dir == 0 ? -1 : +1);

  if (n->balance == bal)
    {
      root->balance = n->balance = 0;

      return rotate_single(root, !dir);
    }
  else
    {
      adjust_balance(root, dir, bal);

      return rotate_double(root, !dir);
    }
}


static avlnode_t* remove_balance(avlnode_t *root, int dir, bool *done)
{
  avlnode_t *n = root->link[!dir];
  int bal = (dir == 0 ? -1 : +1);

  if (n->balance == -bal)
    {
      root->balance = n->balance = 0;

      return rotate_single(root, dir);
    }

  if (n->balance == bal)
    {
      adjust_balance(root, !dir, -bal);

      return rotate_double(root, dir);
    }

  root->balance = -bal;
  n->balance = bal;
  *done = true;

  return rotate_single(root, dir);
}


static avlnode_t* new_node(avltree_t *tree, void *data)
{
  avlnode_t *rn;

  if ((rn = malloc(sizeof *rn)) == NULL)
    return NULL;

  rn->balance = 0;
  rn->data = tree->dup(data);
  rn->link[0] = rn->link[1] = NULL;

  return rn;
}


extern avltree_t* avlnew(cmp_f cmp, dup_f dup, rel_f rel)
{
  avltree_t *rt;

  if ((rt = malloc(sizeof *rt)) == NULL)
    return NULL;

  rt->root = NULL;
  rt->cmp = cmp;
  rt->dup = dup;
  rt->rel = rel;
  rt->size = 0;

  return rt;
}


extern void avldelete(avltree_t *tree)
{
  avlnode_t
    *it = tree->root,
    *save;

  while (it != NULL)
    {
      if (it->link[0] == NULL)
	{
	  save = it->link[1];
	  tree->rel(it->data);
	  free(it);
	}
    else
      {
	save = it->link[0];
	it->link[0] = save->link[1];
	save->link[1] = it;
      }

    it = save;
  }

  free(tree);
}


extern void* avlfind(avltree_t *tree, void *data)
{
  avlnode_t *it = tree->root;

  while (it != NULL)
    {
      int cmp;

      if ((cmp = tree->cmp(it->data, data)) == 0)
	break;

      it = it->link[cmp < 0];
    }

  return ((it == NULL) ? NULL : it->data);
}


extern int avlinsert(avltree_t *tree, void *data)
{
  if (tree->root == NULL)
    {
      if ((tree->root = new_node(tree, data)) == NULL)
	return 0;
    }
  else
    {
      avlnode_t	head = {0}, *s, *t, *p, *q;
      int dir;

      t = &head;
      t->link[1] = tree->root;

      for (s = p = t->link[1] ; ; p = q)
	{
	  dir = tree->cmp(p->data, data) < 0;

	  if ((q = p->link[dir]) == NULL)
	    break;

	  if (q->balance != 0)
	    {
	      t = p;
	      s = q;
	    }
	}

      if ((p->link[dir] = q = new_node(tree, data)) == NULL)
	return 0;

      for (p = s ; p != q ; p = p->link[dir])
	{
	  dir = tree->cmp(p->data, data) < 0;
	  p->balance += dir == 0 ? -1 : +1;
	}

      q = s;

      if (abs(s->balance) > 1)
	{
	  dir = tree->cmp(s->data, data) < 0;
	  s = insert_balance(s, dir);
	}

      if (q == head.link[1])
	tree->root = s;
      else
	t->link[q == t->link[1]] = s;
    }

  ++tree->size;

  return 1;
}


extern int avlerase(avltree_t *tree, void *data)
{
  if (tree->root != NULL)
    {
      avlnode_t *it, *up[HEIGHT_LIMIT];
      int upd[HEIGHT_LIMIT], top = 0;

      it = tree->root;

      for ( ; ; )
	{
	  if (it == NULL)
	    return 0;
	  else if (tree->cmp(it->data, data) == 0)
	    break;

	  upd[top] = tree->cmp(it->data, data) < 0;
	  up[top++] = it;

	  it = it->link[upd[top - 1]];
	}

      if ((it->link[0] == NULL) || (it->link[1] == NULL))
	{
	  int dir = it->link[0] == NULL;

	  if (top != 0)
	    up[top - 1]->link[upd[top - 1]] = it->link[dir];
	  else
	    tree->root = it->link[dir];

	  tree->rel(it->data);
	  free(it);
	}
      else
	{
	  avlnode_t *heir = it->link[1];
	  void *save;

	  upd[top] = 1;
	  up[top++] = it;

	  while (heir->link[0] != NULL)
	    {
	      upd[top] = 0;
	      up[top++] = heir;
	      heir = heir->link[0];
	    }

	  save = it->data;
	  it->data = heir->data;
	  heir->data = save;

	  up[top - 1]->link[up[top - 1] == it] = heir->link[1];

	  tree->rel(heir->data);
	  free(heir);
	}

      bool done = false;

      while (--top >= 0 && !done)
	{
	  up[top]->balance += upd[top] != 0 ? -1 : +1;

	  if (abs(up[top]->balance) == 1)
	    break;
	  else if (abs(up[top]->balance) > 1) {
	    up[top] = remove_balance(up[top], upd[top], &done);

	    if (top != 0)
	      up[top - 1]->link[upd[top - 1]] = up[top];
	    else
	      tree->root = up[0];
	  }
	}

      --tree->size;
    }

  return 1;
}


extern size_t avlsize(avltree_t *tree)
{
  return tree->size;
}


extern avltrav_t *avltnew(void)
{
  return malloc(sizeof(avltrav_t));
}


extern void avltdelete(avltrav_t *trav)
{
  free(trav);
}


static void* start(avltrav_t *trav, avltree_t *tree, int dir)
{
  trav->tree = tree;
  trav->it = tree->root;
  trav->top = 0;

  if (trav->it != NULL)
    {
      while (trav->it->link[dir] != NULL)
	{
	  trav->path[trav->top++] = trav->it;
	  trav->it = trav->it->link[dir];
	}
    }

  return ((trav->it == NULL) ? NULL : trav->it->data);
}


static void* move(avltrav_t *trav, int dir)
{
  if (trav->it->link[dir] != NULL)
    {
      trav->path[trav->top++] = trav->it;
      trav->it = trav->it->link[dir];

      while (trav->it->link[!dir] != NULL)
	{
	  trav->path[trav->top++] = trav->it;
	  trav->it = trav->it->link[!dir];
	}
    }
  else
    {
      avlnode_t *last;

      do
	{
	  if (trav->top == 0)
	    {
	      trav->it = NULL;
	      break;
	    }

	  last = trav->it;
	  trav->it = trav->path[--trav->top];
	}
      while (last == trav->it->link[dir]);
    }

  return ((trav->it == NULL) ? NULL : trav->it->data);
}


extern void* avltfirst(avltrav_t *trav, avltree_t *tree)
{
  return start(trav, tree, 0);
}


extern void* avltlast(avltrav_t *trav, avltree_t *tree)
{
  return start(trav, tree, 1);
}


extern void* avltnext(avltrav_t *trav)
{
  return move(trav, 1);
}


extern void* avltprev(avltrav_t *trav)
{
  return move(trav, 0);
}
