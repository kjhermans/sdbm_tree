/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/**
 * \ingroup bree
 * Frees the resources used by the btree.
 * \param td Non-NULL pointer to an initialized btree structure.
 * \returns Zero on success, or a TDERR_* value on error.
 */
void td_exit
  (td_t* td)
{
  if (td->close) {
    td->close(td);
  }
}

#ifdef __cplusplus
}
#endif
