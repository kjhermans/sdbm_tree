/*
** Copyright 2015 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
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

/**
 * \ingroup btree_private
 *
 * Stores a new value in empty space(s). Returns its offset.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] value Value container of data to be stored.
 * \param[in] refcount Reference count for this value node (min. 1).
 * \param[out] off On successful return, contains the offset within
 * the btree's resource, of the first chunk.
 * \param[in] flags Bits from TDFLG_* values.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_store_value
  (
    td_t* td,
    const tdt_t* value,
    unsigned refcount,
    unsigned* off,
    unsigned flags
  )
{
  unsigned valuesize = value->size;
  unsigned lastchunkoff = 0;

  while (1) {
    unsigned chunkoff = 0;
    unsigned needed = valuesize + sizeof(struct chunkhead);
    unsigned given = needed;

    CHECK(td_claim(td, 0, &chunkoff, &given));
    CHECK2(
      td_store_valuechunk(
        td,
        chunkoff,
        lastchunkoff,
        value->data + (needed - given),
        given - sizeof(struct chunkhead),
        refcount,
        flags
      ),
      td_yield_all(td, chunkoff)
    );
    if (given < needed) {
      valuesize = (needed - given);
    } else if (given == needed) {
      *off = chunkoff;
      return 0;
    } else {
      return TDERR_STRUCT;
    }
    lastchunkoff = chunkoff;
  }
}

#ifdef __cplusplus
}
#endif
