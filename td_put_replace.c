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
 * Replaces a value of a key (not leading to a database scn change).
 * Removes the old chunks needed by value-storage, and claims new chunks.
 * This function is private, and assumes that the database is locked.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param valueptr Offset to the already written value node.
 * \param keyptr Offset to the key node.
 * \param keyhead Key header structure loaded at keyptr.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */ 
int td_put_replace
  (
    td_t*            td,
    const unsigned   valueptr,
    const unsigned   keyptr,
    struct keyhead*  keyhead
  )
{
  CHECK(td_yield_all(td, keyhead->value));
  keyhead->value = valueptr;
  CHECK(td_write_keyhead(td, keyptr, keyhead));
  return 0;
}

#ifdef __cplusplus
}
#endif
