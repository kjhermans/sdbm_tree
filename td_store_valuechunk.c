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
 * Stores a value chunk.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] chunkoff  The offset at which the chunk is to be stored.
 * \param[in] next      Offset of the next chunk header.
 * \param[in] data      Chunk's data.
 * \param[in] size      Chunk's data size.
 * \param[in] refcount  Reference count of the chunk.
 * \param[in] flags     Flags (may contain TDFLG_CHECKSUM).
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_store_valuechunk
  (
    td_t* td,
    unsigned chunkoff,
    unsigned next,
    unsigned char* data,
    unsigned size,
    unsigned refcount,
    unsigned flags
  )
{
  struct chunkhead chunkhead = {
    next,
    size + sizeof(struct chunkhead),
    0,
    refcount
  };
  if (flags & TDFLG_CHECKSUM) {
    td_checksum_create(
      data,
      size,
      &(chunkhead.checksum)
    );
  }
  CHECK(td_write_chunkhead(td, chunkoff, &chunkhead));
  CHECK(
    td_write(
      td,
      chunkoff + sizeof(struct chunkhead),
      data,
      size
    )
  );
  ++(td->header.nchunks);
  return 0;
}

#ifdef __cplusplus
}
#endif
