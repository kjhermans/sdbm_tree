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
 * Reads the header (first bit) or a btree resource.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_read_header
  (td_t* td)
{
  CHECK(
    td_read(
      td,
      0,
      &(td->header),
      sizeof(struct td_header)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif
