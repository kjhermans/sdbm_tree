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
 * Stores an empty value, that is: the zero length string.
 * This special case is optimized for because the claim() function
 * isn't allowed to return a buffer less than the size of a header.
 */
static
int td_store_empty_value
  (td_t* td, unsigned refcount, unsigned* off, unsigned flags)
{
  unsigned chunkoff, given = sizeof(struct chunkhead);
  struct chunkhead h = {
    .next = 0,
    .size = given,
    .checksum = 0,
    .refcount = refcount
  };
  (void)flags;

  if (flags & TDFLG_CHECKSUM) {
    td_checksum_create(0, 0, &(h.checksum));
  }
  CHECK(td_claim(td, 0, &chunkoff, &given));
  if (given != sizeof(struct chunkhead)) {
    return TDERR_STRUCT;
  }
  CHECK(td_write_chunkhead(td, chunkoff, &h));
  ++(td->header.nchunks);
  *off = chunkoff;
  return 0;
}

struct tdvbuf
{
  tdt_t             value;
  unsigned          used;
};

struct tdvvec
{
  struct tdvbuf*    list;
  unsigned          offset;
  unsigned          count;
};

/**
 * Optimization: when the given space is smaller than or equal to
 * the amount of data in my current value, then the current value
 * will not require a copy to be used to store (part of its) data.
 */
static
int td_store_chunk_nocopy
  (
    td_t*           td,
    unsigned        chunkoff,
    unsigned        chunkdatasize,
    unsigned        nextchunk,
    struct tdvvec*  vec,
    unsigned        refcount,
    unsigned        flags
  )
{
  struct tdvbuf* curval = &(vec->list[ vec->offset ]);

  CHECK2(
    td_store_valuechunk(
      td,
      chunkoff,
      nextchunk,
      curval->value.data + curval->value.size - (curval->used + chunkdatasize),
      chunkdatasize,
      refcount,
      flags
    ),
    td_yield_all(td, chunkoff)
  );
  curval->used += chunkdatasize;
  if (curval->used == curval->value.size && vec->offset) {
    --(vec->offset);
  }
  return 0;
}

static
int td_store_chunk_copy
  (
    td_t*           td,
    unsigned        chunkoff,
    unsigned        chunkdatasize,
    unsigned        nextchunk,
    struct tdvvec*  vec,
    unsigned        refcount,
    unsigned        flags
  )
{
  struct tdvbuf* curval = &(vec->list[ vec->offset ]);
  unsigned char chunk[ chunkdatasize ];

  while (chunkdatasize >= curval->value.size - curval->used) {
    chunkdatasize -= (curval->value.size - curval->used);
    memcpy(
      &(chunk[ chunkdatasize ]),
      curval->value.data + curval->used,
      curval->value.size - curval->used
    );
    curval->used = curval->value.size;
    if (vec->offset == 0) {
      if (chunkdatasize) {
        return TDERR_STRUCT;
      }
      break;
    }
    --(vec->offset);
    curval = &(vec->list[ vec->offset ]);
  }
  if (chunkdatasize) {
    memcpy(
      chunk,
      curval->value.data + curval->value.size - chunkdatasize,
      chunkdatasize
    );
    curval->used += chunkdatasize;
  }
  CHECK2(
    td_store_valuechunk(
      td,
      chunkoff,
      nextchunk,
      chunk,
      sizeof(chunk),
      refcount,
      flags
    ),
    td_yield_all(td, chunkoff)
  );
  return 0;
}

static
int td_store_chunk
  (
    td_t*           td,
    unsigned        chunkoff,
    unsigned        chunkdatasize,
    unsigned        nextchunk,
    struct tdvvec*  vec,
    unsigned        refcount,
    unsigned        flags
  )
{
  struct tdvbuf* curval = &(vec->list[ vec->offset ]);

  if (chunkdatasize <= curval->value.size - curval->used) {
    CHECK(
      td_store_chunk_nocopy(
        td, chunkoff, chunkdatasize, nextchunk, vec, refcount, flags
      )
    );
  } else {
    CHECK(
      td_store_chunk_copy(
        td, chunkoff, chunkdatasize, nextchunk, vec, refcount, flags
      )
    );
  }
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
    unsigned valuecount,
    unsigned refcount,
    unsigned* off,
    unsigned flags
  )
{
  unsigned needed = 0;
  unsigned nextchunk = 0;
  unsigned i;
  struct tdvbuf buf[ valuecount ];
  struct tdvvec vec = {
    .list = buf,
    .offset = valuecount - 1,
    .count = valuecount
  };

  if (valuecount == 0) {
    return TDERR_INVAL;
  }
  for (i=0; i < valuecount; i++) {
    needed += value[ i ].size;
    buf[ i ].value = value[ i ];
    buf[ i ].used = 0;
  }
  if (needed == 0) {
    CHECK(td_store_empty_value(td, refcount, off, flags));
    return 0;
  }
  while (needed) {
    unsigned chunkoff, given = needed + sizeof(struct chunkhead), useful;
    CHECK(td_claim(td, 0, &chunkoff, &given));
    if (given < sizeof(struct chunkhead)) {
      return TDERR_STRUCT;
    }
    useful = given - sizeof(struct chunkhead);
    CHECK(
      td_store_chunk(
        td,
        chunkoff,
        useful,
        nextchunk,
        &vec,
        refcount,
        flags
      )
    );
    *off = nextchunk = chunkoff;
    needed -= useful;
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
