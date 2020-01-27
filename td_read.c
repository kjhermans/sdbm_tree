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
 * \ingroup btree_private 
 *
 * Reads a piece of the btree's resource.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] off Offset of the piece to be read within the btree's resource.
 * \param[out] buf The memory buffer to be filled.
 * \param[in] size The size of the piece to be read and copied into the
 * mempory buffer.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */ 
int td_read
  (td_t* td, unsigned off, void* buf, unsigned size)
{
  if (off + size > td->header.size) {
    RETURNERR(TDERR_BOUNDS);
  }
  return td->read(td, off, buf, size, td->ioarg);
}

#ifdef __cplusplus
}
#endif
