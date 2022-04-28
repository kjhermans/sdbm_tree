/*
** Copyright 2022 K.J. Hermans (kees.jan.hermans@gmail.com)
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
 * Puts a value into the tree, under a certain key.
 *
 * Values will be either newly inserted (when the key is not present),
 * replaced (when the key is present and TDFLG_DUPLKEYS has not been set during
 * initialization), or added (when the key is present and TDFLG_DUPLKEYS
 * has been set during initialization).
 *
 * \param td Pointer to an initialized td_t structure.
 * \param key Pointer to an initialized tdt_t structure, containing the key.
 * \param value Pointer to an array of value segments.
 * \param valuecount Non zero number of elements in the value array.
 * \param flags May be used to tweak the way the key/value pair is stored.
 *
 * \returns Zero on success, or any of the errors of the underlying
 * functions.
 */
int td_put_vec
  (
    td_t* td,
    const tdt_t* key,
    const tdt_t* value,
    unsigned valuecount,
    unsigned flags
  )
{
  int r;
  flags |= td->header.flags;
  CHECK(td_lock(td));
  r = td_put_locked_key(td, key, value, valuecount, flags);
  CHECK(td_unlock(td));
  return r;
}

#ifdef __cplusplus
}
#endif
