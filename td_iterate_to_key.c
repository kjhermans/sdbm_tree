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
 * Recursive function.
 * Iterates to the point in the dbm at or nearest to before the sought key.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param path Search path built up to this point.
 * \param key Non-NULL pointer to a potentially uninitialized tdt.
 * \param partial Boolean. Whether or not matches are allowed to be partial.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_iterate_to_key
  (td_t* td, struct path* path, const tdt_t* key, int partial)
{
  TD_PATH_VALID(path);
  CHECK(td_read_keyhead(td, path->head->ptr, &(path->head->keyhead)));
  CHECK(td_iterate_compare(td, path->head, key, partial)); 
  if (path->head->cmp < 0) {
    unsigned previous;
    if ((previous = path->head->keyhead.previous) != 0) {
      ++(path->size);
      ++(path->head);
      TD_PATH_VALID(path);
      path->head->ptr = previous;
      CHECK(td_iterate_to_key(td, path, key, partial));
    } 
  } else if (path->head->cmp > 0) {
    unsigned next;
    if ((next = path->head->keyhead.next) != 0) {
      ++(path->size);
      ++(path->head);
      TD_PATH_VALID(path);
      path->head->ptr = next;
      CHECK(td_iterate_to_key(td, path, key, partial));
    } 
//  } else if (partial) {
  } else {
    while (1) {
      int r;
      struct path orig_path = *path;
      switch (r = td_path_previous(td, path)) {
      case 0:
        if (path->head != NULL && path->size != 0) {
          CHECK(td_iterate_compare(td, path->head, key, partial));
          if (path->head->cmp != 0) {
            *path = orig_path;
            return 0;
          }
          break;
        }
        __attribute__ ((fallthrough));
      case TDERR_NOTFOUND:
        *path = orig_path;
        return 0;
      default:
        return r;
      }
    }
  }
  return 0;
} 

#ifdef __cplusplus
}
#endif
