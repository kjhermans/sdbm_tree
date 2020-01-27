/*
** Copyright 2015 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
int td_pop_locked
  (td_t* td, tdt_t* key, tdt_t* value, unsigned flags)
{
  CHECK(td_read_header(td));
  if (td->header.top != 0) {
    struct path path = TD_PATH_INIT(td->header.top);
    CHECK(td_iterate_to_min(td, &path));
    if (key) {
      CHECK(
        td_read_keydata(td, path.head->ptr, &(path.head->keyhead), key, flags)
      );
    }
    CHECK(td_del_pathhead(td, &path, value, flags));
    return 0;
  } else {
    return TDERR_NOTFOUND;
  }
}

/** 
 * \ingroup btree_private 
 *
 * Pops the left-most key/value pair out of the tree, and optionally
 * returns them.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param key pointer to a potentially uninitialized tdt.
 * \param value pointer to a potentially uninitialized tdt.
 * \param flags Bits from the TDFLG_* values.
 *
 * \returns Zero on success, or a TDERR_NOTFOUND when the database
 * is empty, or any other TDERR_* value on error.
 */ 
int td_pop
  (td_t* td, tdt_t* key, tdt_t* value, unsigned flags)
{
  int r;
  CHECK(td_lock(td));
  r = td_pop_locked(td, key, value, flags);
  CHECK(td_unlock(td));
  return r;
}

#ifdef __cplusplus
}
#endif
