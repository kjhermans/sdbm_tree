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
 * Set administrated empty space to zero.
 *
 * \param td Non-NULL pointer to initialized td_t structure.
 * \param off Offset to the chunk to be zeroised.
 *
 * \returns Zero on success, or non-zero on failure.
 */
int td_yield_set_zero
  (td_t* td, unsigned off)
{
  unsigned char zerobuf[ 256 ];
  unsigned size;
  memset(zerobuf, 0, sizeof(zerobuf));
  CHECK(td_read_uint(td, off + sizeof(unsigned), &size));
  if (size < (2 * sizeof(unsigned))) {
    return TDERR_BOUNDS;
  }
  off += (2 * sizeof(unsigned));
  size -= (2 * sizeof(unsigned));
  while (size) {
    unsigned l = ((size > sizeof(zerobuf)) ? sizeof(zerobuf) : size);
    CHECK(td_write(td, off, zerobuf, l));
    off += l;
    size -= l;
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
