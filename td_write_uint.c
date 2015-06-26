/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/**
 * \ingroup btree_private
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
