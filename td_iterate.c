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
 * Searches down the tree to the point of, or nearest to, a key.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param path Container of the path down the tree to the appropriate node.
 * \param key Non-NULL pointer to a potentially uninitialized tdt.
 * \param partial Boolean. Whether or not matches are allowed to be partial.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_iterate
  (td_t* td, struct path* path, const tdt_t* key, int partial)
{
  CHECK(td_iterate_to_key(td, path, key, partial));
  if (path->size > td->maxdepth) {
    td->maxdepth = path->size;
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
