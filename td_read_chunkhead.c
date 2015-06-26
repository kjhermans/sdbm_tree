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
 * Reads a node header from the btree's resource.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] off Offset of the piece to be read within the btree's resource.
 * \param[out] chunkhead The chunkhead found at the offset.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_read_chunkhead
  (td_t* td, unsigned off, struct chunkhead* chunkhead)
{
  if (off < sizeof(struct td_header)) {
    RETURNERR(TDERR_BOUNDS);
  }
  CHECK(
    td_read(
      td,
      off,
      chunkhead,
      sizeof(struct chunkhead)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif
