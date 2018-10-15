/*
  AVL balanced tree library

  - Created (Julienne Walker): June 17, 2003
  - Modified (Julienne Walker): September 24, 2005
  - Modified (J.J. Green, 2018)

  This code is in the public domain. Anyone may use it or change
  it in any way that they see fit. The author assumes no
  responsibility for damages incurred through use of the original
  code or any variations thereof.

  It is requested, but not required, that due credit is given to
  the original author and anyone who has modified the code through
  a header comment, such as this one.
*/

#ifndef AVLTREE_H
#define AVLTREE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

/* opaque types */

typedef struct avltree_t avltree_t;
typedef struct avltrav_t avltrav_t;

/* user-defined item handling */

typedef int (*cmp_f)(const void *p1, const void *p2);
typedef void* (*dup_f)(void *p);
typedef void (*rel_f)(void *p);

/* tree functions */

extern avltree_t *avlnew(cmp_f, dup_f, rel_f);
extern void avldelete(avltree_t*);
extern void* avlfind(avltree_t*, void*);
extern bool avlinsert(avltree_t*, void*);
extern bool avlerase(avltree_t*, void*);
extern size_t avlsize(avltree_t*);
extern void avliter(avltree_t*, void (*)(void*, void*), void*);

/* traversal */

extern avltrav_t* avltnew(void);
extern void avltdelete(avltrav_t*);
extern void* avltfirst(avltrav_t*, avltree_t*);
extern void* avltlast(avltrav_t*, avltree_t*);
extern void* avltnext(avltrav_t*);
extern void* avltprev(avltrav_t*);

#endif
