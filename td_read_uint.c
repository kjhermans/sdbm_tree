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
 * Reads a single unsigned integer off the btree's resource.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] off Offset of the integer to be read within the btree's resource.
 * \param[out] data On successful return, contains the integer.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_read_uint
  (td_t* td, unsigned off, unsigned* data)
{
  CHECK(
    td_read(
      td,
      off,
      data,
      sizeof(unsigned)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif
