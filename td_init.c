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
 * \ingroup btree
 *
 * Initializes (sets to all zero) a btree structure.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_init
  (td_t* td)
{
  memset(td, 0, sizeof(td_t));
  return 0;
}

#ifdef __cplusplus
}
#endif
