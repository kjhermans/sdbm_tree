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
 * \ingroup bree_private
 *
 * Extends the btree's resource by a certain amount to fit new data.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param wanted Amount of space wanted.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 * Most notably, it may return TDERR_SPACE, which is not necessarily
 * all that grave; the btree is otherwise intact, and API users may choose
 * to recover from it gracefully.
 */
int td_extend
  (td_t* td, unsigned wanted)
{
  unsigned granted = td->extend(td, wanted, td->extendarg);
  if (TD_OFF_MAX - granted < td->header.size) {
    return TDERR_SPACE;
  }
  if (granted > sizeof(struct chunkhead)) {
    unsigned oldsize = td->header.size;
    struct chunkhead chunkhead = {
      0,
      granted,
      0,
      0
    };
    td->header.size += granted;
    CHECK(td_write_chunkhead(td, oldsize, &chunkhead));
    CHECK(td_yield(td, oldsize));
    return 0;
  } else {
    return TDERR_SPACE;
  }
}

#ifdef __cplusplus
}
#endif
