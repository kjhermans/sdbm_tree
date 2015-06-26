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
 * Administrates space yielded to the empty list.
 * If the flag TDFLG_WIPEDELETED has been set on initialization,
 * then the space of the chunk is set to all zero before being
 * added to the empty list.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param off Offset of the chunk to be yielded to the empty list.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_yield
  (td_t* td, unsigned off)
{
  unsigned next = td->header.off_e;
  td->header.off_e = off;
  if (td->header.flags & TDFLG_WIPEDELETED) {
    CHECK(td_yield_set_zero(td, off));
  }
  CHECK(td_write_uint(td, off, next));
  ++(td->header.nempties);
  return 0;
}

#ifdef __cplusplus
}
#endif
