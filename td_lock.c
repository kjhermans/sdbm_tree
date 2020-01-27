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
 * Locks the btree.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */ 
int td_lock
  (td_t* td)
{
  if (td->lock) {
    if (++(td->locked) == 1) {
      return td->lock(td, 1, td->lockarg);
    }
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
