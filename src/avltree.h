#ifndef AVLTREE_H
#define AVLTREE_H

/*
  AVL balanced tree library

    > Created (Julienne Walker): June 17, 2003
    > Modified (Julienne Walker): September 24, 2005

  This code is in the public domain. Anyone may
  use it or change it in any way that they see
  fit. The author assumes no responsibility for
  damages incurred through use of the original
  code or any variations thereof.

  It is requested, but not required, that due
  credit is given to the original author and
  anyone who has modified the code through
  a header comment, such as this one.
*/

#include <stddef.h>

/* Opaque types */

typedef struct avltree_t avltree_t;
typedef struct avltrav_t avltrav_t;

/* User-defined item handling */

typedef int (*cmp_f)(const void *p1, const void *p2);
typedef void* (*dup_f)(void *p);
typedef void (*rel_f)(void *p);

/* AVL tree functions */

avltree_t *avlnew(cmp_f, dup_f, rel_f);
void avldelete(avltree_t*);
void *avlfind(avltree_t*, void*);
int avlinsert(avltree_t*, void*);
int avlerase(avltree_t*, void*);
size_t avlsize(avltree_t*);

/* Traversal functions */

avltrav_t* avltnew(void);
void avltdelete(avltrav_t*);
void* avltfirst(avltrav_t*, avltree_t*);
void* avltlast(avltrav_t*, avltree_t*);
void* avltnext(avltrav_t*);
void* avltprev(avltrav_t*);

#endif
