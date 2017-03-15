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
 * Reads a key header.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] off Offset of the key header to be read.
 * \param[out] keyhead On successful return, contains the key header.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_read_keyhead
  (td_t* td, unsigned off, struct keyhead* keyhead)
{
  if (off < sizeof(struct td_header)) {
    TDLOG("libbtree: td_read_keyhead: Attempt read off=%u\n", off);
    RETURNERR(TDERR_BOUNDS);
  }
  CHECK(
    td_read(
      td,
      off,
      keyhead,
      sizeof(struct keyhead)
    )
  );
  TDLOG("%s At %u, value=%u, size=%u, checksum=%u, next=%u, previous=%u\n",
    __FILE__,
    off,
    keyhead->value,
    keyhead->size,
    keyhead->checksum,
    keyhead->next,
    keyhead->previous
  );
  return 0;
}

#ifdef __cplusplus
}
#endif
