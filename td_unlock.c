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
 * Unlocks a tree.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 *
 * \returns Zero on success, or a fatal error on failure.
 */
int td_unlock
  (td_t* td)
{
  if (td->lock) {
    if (--(td->locked) == 0) {
      return td->lock(td, 0, td->lockarg);
    }
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
