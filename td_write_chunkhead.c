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
 * Writes the head of a chunk. A chunk is a simple container of a part
 * of data.
 *
 * \param td
 * \param[in] off Offset of the chunk
 * \param[in] chunkhead Chunk header to be written.
 *
 * \returns Zero on success, or a fatal TDERR_* value on error.
 */
int td_write_chunkhead
  (td_t* td, unsigned off, const struct chunkhead* chunkhead)
{
  CHECK(
    td_write(
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
