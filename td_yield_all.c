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
 * Yields all chunks of a given sequence. This is used to remove a value.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param ptr Offset to the first chunk.
 *
 * \returns Zero on success or a TDERR_* value on error.
 */
int td_yield_all
  (td_t* td, unsigned ptr)
{
  while (ptr) {
    unsigned next;
    CHECK(td_read_uint(td, ptr, &next));
    CHECK(td_yield(td, ptr));
    --(td->header.nchunks);
    ptr = next;
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
