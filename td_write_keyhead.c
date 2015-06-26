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
 * Writes the header of a key to the btree's resource at a certain offset.
 *
 * \param td Non-NULL to an initialized pointer to a btree structure.
 * \param[in] off Offset of the keyhead within the btree resource.
 * \param[in] keyhead Non-NULL. The fully initialized key header to be written.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_write_keyhead
  (td_t* td, unsigned off, const struct keyhead* keyhead)
{
  CHECK(
    td_write(
      td,
      off,
      keyhead,
      sizeof(struct keyhead)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif
