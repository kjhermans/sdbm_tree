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
 * See documentation for td_rotate_left().
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] parentptr Offset of the parent key node.
 * \param[in] parent Key header of the parent node.
 * \param[in] ptr Offset of the key node itself.
 * \param[in] node Key node itself.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_rotate_right
  (
    td_t* td,
    unsigned parentptr,
    struct keyhead* parent,
    unsigned ptr,
    struct keyhead* node
  )
{
  unsigned leftptr;
  struct keyhead leftnode;
//fprintf(stderr, "Rotate right.\n");
  if ((leftptr = node->previous) == 0) {
    RETURNERR(TDERR_BOUNDS);
  }
  CHECK(td_read_keyhead(td, leftptr, &leftnode));
  node->previous = leftnode.next;
  leftnode.next = ptr;
  if (parentptr) {
    if (parent->next == ptr) {
      parent->next = leftptr;
    } else if (parent->previous == ptr) {
      parent->previous = leftptr;
    } else {
      RETURNERR(TDERR_STRUCT);
    }
    CHECK(td_write_keyhead(td, parentptr, parent));
  } else {
    td->header.top = leftptr;
    CHECK(td_write_header(td));
  }
  CHECK(td_write_keyhead(td, leftptr, &leftnode));
  CHECK(td_write_keyhead(td, ptr, node));
  return 0;
}

#ifdef __cplusplus
}
#endif
