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
 * Reads a value off the btree's resource.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] ptr Offset of the first chunk of the value to be read.
 * \param[out] value On successful return, contains the value.
 * \param[in] flags Bits from the TDFLG_* values.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_read_value
  (td_t* td, unsigned ptr, tdt_t* value, unsigned flags)
{
  if (ptr < sizeof(struct td_header)) {
    RETURNERR(TDERR_BOUNDS);
  }
  flags |= td->header.flags;
  int reallocating = (
    value->data == 0 &&
    value->size == 0 &&
    (flags & TDFLG_ALLOCTDT) &&
    td->realloc != 0
  );
  unsigned off = 0;
  while (ptr) {
    struct chunkhead chunkhead;
    CHECK(td_read_chunkhead(td, ptr, &chunkhead));
    unsigned chunkdatasize = chunkhead.size - sizeof(struct chunkhead);
    char chunkdata[ chunkdatasize ];
    CHECK(td_read(td, ptr + sizeof(chunkhead), chunkdata, chunkdatasize));
    if (flags & TDFLG_CHECKSUM) {
      CHECK(td_checksum_verify(chunkdata, chunkdatasize, chunkhead.checksum));
    }
    unsigned wantedsize = off + chunkdatasize;
    if (reallocating && wantedsize > value->size) {
      void* mem = td->realloc(td, value->data, wantedsize, td->reallocarg);
      if (mem) {
        value->data = mem;
        value->size = wantedsize;
      } else {
        RETURNERR(TDERR_NOMEM);
      }
    }
    if (wantedsize > value->size) {
      memcpy(value->data + off, chunkdata, value->size - off);
      return 0;
    } else {
      memcpy(value->data + off, chunkdata, chunkdatasize);
      off += chunkdatasize;
    }
    ptr = chunkhead.next;
  }
  value->size = off;
  return 0;
}

#ifdef __cplusplus
}
#endif
