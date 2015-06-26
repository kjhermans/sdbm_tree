/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/**
 * \ingroup btree_private
 *
 * Debugs a path to stderr. A path is what a cursor uses to move through a dbm.
 *
 * \param path The path to be debugged.
 *
 * \returns Zero.
 */
int td_debug_path
  (struct path* path)
{
  unsigned i;
  fprintf(stderr, "PATH: size=%u head=%s\n",
    path->size,
    (path->head == ((path->size) ? (&(path->stack[ path->size-1 ])) : 0))
    ? "correct" : "INCORRECT"
  );
  for (i=0; i < path->size; i++) {
    fprintf(stderr,
      "%u: ptr=%u, keyhead=(previous=%u, next=%u), cmp=%d\n", 
      i,
      path->stack[ i ].ptr,
      path->stack[ i ].keyhead.previous,
      path->stack[ i ].keyhead.next,
      path->stack[ i ].cmp
    );
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
