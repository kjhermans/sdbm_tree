/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

#include "td_private.h"

static
int td_update_valuechunk
  (
    td_t* td,
    unsigned chunkoff,
    unsigned next
  )
{
  CHECK(td_write_uint(td, chunkoff, next));
  return 0;
}

static
int td_store_valuechunk
  (
    td_t* td,
    unsigned chunkoff,
    unsigned char* data,
    unsigned size,
    unsigned refcount,
    unsigned flags
  )
{
  struct chunkhead chunkhead = {
    0, // next is unknown at this stage, it will be written during update
    size + sizeof(struct chunkhead),
    0, // checksum
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
 * \param[in] fd File descr from which chunks will be read until EOF, as value.
 * \param[in] refcount Reference count for this value node (min. 1).
 * \param[out] off On successful return, contains the offset within
 * the btree's resource, of the first chunk.
 * \param[in] flags Bits from TDFLG_* values.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_store_value_stream
  (
    td_t* td,
    int fd,
    unsigned refcount,
    unsigned* off,
    unsigned flags
  )
{
  unsigned lastchunkoff = 0;

  (*off) = 0;
  while (1) {
    unsigned char chunk[ 4096 - sizeof(struct chunkhead) ];
    int r, o = 0;

    if ((r = read(fd, chunk, sizeof(chunk))) < 0) {
      RETURNERR(TDERR_SPACE);
    } else if (r == 0) {
      if ((*off) != 0) {
        return 0;
      }
    }

    while (o < r) {
      unsigned chunkoff = 0;
      unsigned needed = (r - o) + sizeof(struct chunkhead);
      unsigned given = needed;
  
      CHECK(td_claim(td, 0, &chunkoff, &given));
      
      if ((*off) == 0) {
        (*off) = chunkoff;
      }
      CHECK2(
        td_store_valuechunk(
          td,
          chunkoff,
          &(chunk[ o ]),
          given - sizeof(struct chunkhead),
          refcount,
          flags
        ),
        td_yield_all(td, (*off))
      );
      if (lastchunkoff) {
        CHECK2(
          td_update_valuechunk(
            td,
            lastchunkoff,
            chunkoff
          ),
          td_yield_all(td, (*off))
        );
      }
      if (r == 0) {
        return 0;
      }
      o += (given - sizeof(struct chunkhead));
      lastchunkoff = chunkoff;
    }
  }
}

#ifdef __cplusplus
}
#endif
