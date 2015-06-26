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
 * Write a piece of data into the btree resource at a given offset,
 * with a given size.
 *
 * \param td
 * \param[in] off Offset within the resource to write to.
 * \param[in] buf Non-NULL pointer. Buffer to write within the resource.
 * \param[in] size Size of the buffer.
 *
 * \returns Zero on success, or any TDERR_* value on error.
 */
int td_write
  (td_t* td, unsigned off, const void* buf, unsigned size)
{
  if (off + size > td->header.size) {
    RETURNERR(TDERR_BOUNDS);
  }
  return td->write(td, off, buf, size, td->ioarg);
}

#ifdef __cplusplus
}
#endif
