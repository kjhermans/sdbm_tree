/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/**
 * \ingroup btree_private
 * Recursive function.
 * Rebalances a node by shifting weight away from the heavier child.
 * First determines weight by accessing the children, then shift them
 * if the children are out of balance and it can be fixed (i.e. the
 * difference in weight between them is bigger than one).
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] parentptr Offset of the parent key node.
 * \param[in] parentnode Key header of the parent node.
 * \param[in] ptr Offset of the key node itself.
 * \param[out] weight Filled with the added weight of the children + 1
 * on return.
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_rebalance_node
  (
    td_t* td,
    unsigned parentptr,
    struct keyhead* parentnode,
    unsigned ptr,
    unsigned* weight
  )
{
  unsigned leftweight = 0, rightweight = 0;
  struct keyhead node;
  CHECK(td_read_keyhead(td, ptr, &node));
  if (node.previous != 0) {
    CHECK(td_rebalance_node(td, ptr, &node, node.previous, &leftweight));
  }
  if (node.next != 0) {
    CHECK(td_rebalance_node(td, ptr, &node, node.next, &rightweight));
  }
  if (leftweight > rightweight + 1) {
    CHECK(td_rotate_right(td, parentptr, parentnode, ptr, &node));
  } else if (rightweight > leftweight + 1) {
    CHECK(td_rotate_left(td, parentptr, parentnode, ptr, &node));
  }
  (*weight) = (leftweight + rightweight + 1);
  return 0;
}

#ifdef __cplusplus
}
#endif
