/*
** Copyright 2024 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
int td_get_locked
  (td_t* td, tdt_t* key, tdt_t* value, unsigned flags)
{
  struct path path;
  CHECK(td_read_header(td));
  if (td->header.top == 0) {
    return TDERR_NOTFOUND;
  }
  path = TD_PATH_INIT(td->header.top);
  CHECK(td_iterate(td, &path, key, flags));
  if (!(flags & TDFLG_EXACT) || path.head->cmp == 0) {
    if (value) {
      CHECK(td_read_value(td, path.head->keyhead.value, value, flags));
    }
    if (flags & TDFLG_ALLOCKEY) {
      CHECK(td_read_keydata(td, path.head->ptr, &(path.head->keyhead), key, 0));
    }
    return 0;
  }
  return TDERR_NOTFOUND;
}

/**
 * \ingroup btree
 *
 * Gets a key/value pair from a btree.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param key Non-NULL pointer to an initialized tdt.
 * \param value Potentially NULL pointer to a potentially uninitialized tdt.
 * \param flags Bits from the TDFLG_* values.
 *
 * Note that this function will behave like cursor wrt the TDFLG_EXACT
 * and TDFLG_PARTIAL. That is to say:
 *
 * - When TDFLG_PARTIAL|TDFLG_EXACT is specified, any match that is both
 *   partial (ie matches where the search key is smaller than or equal to
 *   the found key in length) and exact (ie it matches exactly for that
 *   length) will return zero. If such a key does not exist, it will
 *   return TDERR_NOTFOUND.
 * - When only TDFLG_PARTIAL is specified, any match that is at least
 *   partial will return zero. If such a key does not exist, it will
 *   return TDERR_NOTFOUND.
 * - When only TDFLG_EXACT is specified, the found key must match the search
 *   key both in length and content, and any other situation will return
 *   TDERR_NOTFOUND.
 * - When none of TDFLG_EXACT or TDFLG_PARTIAL are given, the nearest match
 *   will be returned. If the database is empty, TDERR_NOTFOUND is returned.
 *
 * Also note that, since this library will never overwrite
 * a given buffer's size, this poses a problem wrt partial fetches of keys:
 * you will not be able to specify that you want to search partially,
 * but retrieve wholly. To fix this, you must do the following:
 *
 * - Specify both TDFLG_ALLOCKEY with the flags bitmask.
 * - On successful return (and only then), the pointer in key->data will
 *   be the result of a call to malloc(). Obviously, key->size will represent
 *   the amount of malloced data.
 * - After use of key->data, don't forget to free() it.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_get
  (td_t* td, tdt_t* key, tdt_t* value, unsigned flags)
{
  int r;
  flags |= td->header.flags;
  CHECK(td_lock(td));
  r = td_get_locked(td, key, value, flags);
  CHECK(td_unlock(td));
  return r;
}

#ifdef __cplusplus
}
#endif
