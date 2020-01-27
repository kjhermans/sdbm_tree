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
 * Writes a unsigned integer into the resource at a given position.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param off Offset of the integer to be written.
 * \param value Value of the integer to be written.
 *
 * \returns Zero on success or a TDERR_* value on error.
 */
int td_write_uint
  (td_t* td, unsigned off, unsigned value)
{
  CHECK(
    td_write(
      td,
      off,
      &value,
      sizeof(unsigned)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif
