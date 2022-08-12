/*
** Copyright 2022 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
int tdc_del_locked
  (tdc_t* tdc)
{
  return td_del_pathhead(tdc->td, &(tdc->path), 0, 0);
}

/**
 * \ingroup btree
 *
 * Removes the object at the cursor.
 *
 * \par NOTE
 * This function invalidates the cursor after successful execution,
 * because the structure of the tree cannot be relied on anymore.
 * (ie You can call this function once).
 *
 * \param tdc    Pointer to an initialized tdc_t structure.
 * \param key    Optional pointer to an initialized dbt_t structure.
 *               When non-NULL, is used to return the key at the cursor.
 * \param value  Optional pointer to an initialized dbt_t structure.
 *               When non-NULL, is used to return the value at the cursor.
 * \param flags  Flags; these only pertain to the 'get' portion of this
 *               function and are ignored otherwise.
 *               if TDFLG_ALLOCTDT is given, and the key or value
 *               size is NULL, room in them will be allocated using the
 *               allocator function.
 *
 * \returns Zero on success, TDERR_NOTFOUND if the cursor is not positioned
 *          somewhere viable, or TDERR_INVAL when the System Change Number
 *          has invalidated the cursor (cursor is stale).
 *          Or any other errors of any underlying functions.
 */
int tdc_del
  (tdc_t* tdc, tdt_t* key, tdt_t* value, unsigned flags)
{
  int r;
  flags |= tdc->td->header.flags;
  CHECK(td_lock(tdc->td));
  if (key || value) {
    if ((r = tdc_get_locked(tdc, key, value, flags)) == 0) {
      r = tdc_del_locked(tdc);
    }
  } else {
    if ((r = td_read_header(tdc->td)) == 0) {
      r = tdc_del_locked(tdc);
    }
  }
  CHECK(td_lock(tdc->td));
  return r;
}

#ifdef __cplusplus
}
#endif
