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
 * Iterates to the smallest (left-most) key.
 * Recursive function.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param path Search path built up to this point.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */ 
int td_iterate_to_min
  (td_t* td, struct path* path)
{
  unsigned previous;
  TD_PATH_VALID(path);
  CHECK(
    td_read_keyhead(td, path->head->ptr, &(path->head->keyhead))
  );
  if ((previous = path->head->keyhead.previous) == 0) {
    path->head->cmp = 0;
  } else {
    path->head->cmp = -1;
    ++(path->size);
    ++(path->head);
    TD_PATH_VALID(path);
    path->head->ptr = previous;
    CHECK(td_iterate_to_min(td, path));
  }
  return 0;
} 

#ifdef __cplusplus
}
#endif
