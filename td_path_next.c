/*
** Copyright 2015 K.J. Hermans (kees.jan.hermans@gmail.com)
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
 * Moves a path structure (used in normal searches and cursors)
 * to the next node.
 *
 * \param td Non-NULL pointer to initialized td_t structure.
 * \param path Non-NULL pointer to initialized struct path.
 *
 * \returns Zero on success, or non-zero on error.
 */
int td_path_next
  (td_t* td, struct path* path)
{
  while (path->size) {
    unsigned next;
    switch (path->head->cmp) {
    case -1:
      path->head->cmp = 0;
      return 0;
    case 0:
      if ((next = path->head->keyhead.next) != 0) {
        if (path->size < TD_MAXPATH) {
          path->head->cmp = 1;
          ++(path->size);
          ++(path->head);
          path->head->ptr = next;
          CHECK(td_iterate_to_min(td, path));
          return 0;
        } else {
          RETURNERR(TDERR_INVAL);
        }
      }
    case 1:
      break;
    default:
      RETURNERR(TDERR_STRUCT);
    }
    --(path->size);
    if (path->size) {
      --(path->head);
    } else {
      path->head = 0;
    }
  }
  return TDERR_NOTFOUND;
}

#ifdef __cplusplus
}
#endif
