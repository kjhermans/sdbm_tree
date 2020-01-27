/*
** Copyright 2015 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

#include <unistd.h>

static
int td_read_value_stream_write_chunk
  (int fd, unsigned char* chunk, unsigned chunksize)
{
  unsigned off = 0;
  int w;

  while (off < chunksize) {
    if ((w = write(fd, chunk + off, chunksize - off)) > 0) {
      off += w;
    } else {
      RETURNERR(TDERR_SPACE);
    }
  }
  return 0;
}

/**
 * \ingroup btree_private
 *
 * Reads a value off the btree's resource.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] ptr Offset of the first chunk of the value to be read.
 * \param[out] fd File descriptor to which the value will be written.
 * \param[in] flags Bits from the TDFLG_* values.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_read_value_stream
  (td_t* td, unsigned ptr, int fd, unsigned flags)
{
  if (ptr < sizeof(struct td_header)) {
    RETURNERR(TDERR_BOUNDS);
  }
  flags |= td->header.flags;
  while (ptr) {
    struct chunkhead chunkhead;
    CHECK(td_read_chunkhead(td, ptr, &chunkhead));
    unsigned chunkdatasize = chunkhead.size - sizeof(struct chunkhead);
    unsigned char chunkdata[ chunkdatasize ];
    CHECK(td_read(td, ptr + sizeof(chunkhead), chunkdata, chunkdatasize));
    if (flags & TDFLG_CHECKSUM) {
      CHECK(td_checksum_verify(chunkdata, chunkdatasize, chunkhead.checksum));
    }
    CHECK(td_read_value_stream_write_chunk(fd, chunkdata, chunkdatasize));
    ptr = chunkhead.next;
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
