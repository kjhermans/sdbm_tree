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
 * Removes the node that is at the head of a given search path,
 * optionally filling provided key and value containers.
 * This is a private function that assumes the database has been locked.
 *
 * \param[in] td Pointer to an initialized td_t structure.
 * \param[in] path
 * \param[out] value Pointer to an initialized tdt_t structure,
 * which will be filled
 * with the removed value, or NULL if you're just interested in removal.
 * \param[in] flags May be specified to retrieve the deleted value.
 *
 * \returns Zero on success, ...
 */
int td_del_pathhead
  (td_t* td, struct path* path, tdt_t* value, unsigned flags)
{
  unsigned child_ptr = 0;
  struct stackelt* parent = 0;
  if (value) {
    CHECK(td_read_value(td, path->head->keyhead.value, value, flags));
  }
  if (path->size > 1) {
    parent = path->head - 1;
  }
  if (path->head->keyhead.previous !=0 && path->head->keyhead.next != 0) {
    struct path subpath = TD_PATH_INIT(path->head->keyhead.next);
    CHECK(td_iterate_to_min(td, &subpath));
    subpath.head->keyhead.previous = path->head->keyhead.previous;
    CHECK(td_write_keyhead(td, subpath.head->ptr, &(subpath.head->keyhead)));
    child_ptr = path->head->keyhead.next;
  } else if (path->head->keyhead.previous != 0 &&
             path->head->keyhead.next == 0)
  {
    child_ptr = path->head->keyhead.previous;
  } else if (path->head->keyhead.previous == 0 &&
             path->head->keyhead.next != 0)
  {
    child_ptr = path->head->keyhead.next;
  }
  if (parent == 0) {
    td->header.top = child_ptr;
  } else {
    if (parent->cmp < 0) {
      parent->keyhead.previous = child_ptr;
    } else if (parent->cmp > 0) {
      parent->keyhead.next = child_ptr;
    } else {
      RETURNERR(TDERR_STRUCT);
    }
    CHECK(td_write_keyhead(td, parent->ptr, &(parent->keyhead)));
  }
  --(td->header.nentries);
  CHECK(td_unlink_value(td, path->head->keyhead.value));
  CHECK(td_yield(td, path->head->ptr));
  CHECK(td_rebalance(td));
  if ((++(td->header.scn) % 8) == 0) {
    CHECK(td_defrag(td));
  }
  CHECK(td_write_header(td));
  return 0;
}

#ifdef __cplusplus
}
#endif
