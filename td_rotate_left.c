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
 * Rotates the current node 'to the right' that is, toward the 'previous'
 * side, because the weight at the node's 'next' pointer is bigger than
 * one plus the weight at the node's 'previous' pointer.
 * There are two ways in which this rotation can take place, depending
 * on whether the node's next node has a previous node or not.
 
\par Simple
<pre>

             Original:                       Becomes:

             parent                          parent
               |                               |
              node                           right
              /  \                           /   \
             a    right                    node   b
                  /   \                    /  \
                 0     b                  a    0

             Order:                          Order:

         a node right b                   a node right b

</pre>

\par More Complex
<pre>

             Original:                       Becomes:

             parent                          parent
               |                               |
              node                           rightp
              /  \                           /   \
             a    right                    node   right
                  /   \                    /  \    /  \
               rightp  b                  a    c  d    b
               /   \
              c     d

             Order:                          Order:

      a node c rightp d right b       a node c rightp d right b

</pre>

 * Note that only 'node' and 'right' must exist; 'a', 'rightp', 'c', 'd'
 * and 'b' may all be non-existent.
 * Also note that only the non-single letter nodes have to be re-written
 * to the dbm's resource.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] parentptr Offset of the parent key node.
 * \param[in] parent Key header of the parent node.
 * \param[in] ptr Offset of the key node itself.
 * \param[in] node Key node itself.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_rotate_left
  (
    td_t* td,
    unsigned parentptr,
    struct keyhead* parent,
    unsigned ptr,
    struct keyhead* node
  )
{
  unsigned rightptr;
  struct keyhead rightnode;

  if ((rightptr = node->next) == 0) {
    RETURNERR(TDERR_BOUNDS);
  }
  CHECK(td_read_keyhead(td, rightptr, &rightnode));
  node->next = rightnode.previous;
  rightnode.previous = ptr;
  if (parentptr) {
    if (parent->next == ptr) {
      parent->next = rightptr;
    } else if (parent->previous == ptr) {
      parent->previous = rightptr;
    } else {
      RETURNERR(TDERR_STRUCT);
    }
    CHECK(td_write_keyhead(td, parentptr, parent));
  } else {
    td->header.top = rightptr;
    CHECK(td_write_header(td));
  }
  CHECK(td_write_keyhead(td, rightptr, &rightnode));
  CHECK(td_write_keyhead(td, ptr, node));
  return 0;
}

#ifdef __cplusplus
}
#endif
