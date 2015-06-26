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
 * Writes the file header to the first position of the resource.
 *
 * \param td Non-NULL initialized pointer to a btree structure.
 *
 * \return Zero on success, or any TDERR_* value on error.
 */
int td_write_header
  (td_t* td)
{
  CHECK(
    td_write(
      td,
      0,
      (&(td->header)),
      sizeof(struct td_header)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif
