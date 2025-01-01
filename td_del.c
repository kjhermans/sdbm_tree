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
int td_del_one
  (td_t* td, const tdt_t* key, tdt_t* value, unsigned flags)
{
  CHECK(td_read_header(td));
  if (td->header.top == 0) {
    return TDERR_NOTFOUND;
  } else {
    struct path path = TD_PATH_INIT(td->header.top);
    CHECK(td_iterate(td, &path, key, flags));
    if (path.head->cmp != 0) {
      return TDERR_NOTFOUND;
    } 
    CHECK(td_del_pathhead(td, &path, value, flags));
    return 0;
  }
}

static
int td_del_locked
  (td_t* td, const tdt_t* key, tdt_t* value, unsigned flags)
{
  if (flags & TDFLG_DELETEALL) {
    CHECK(td_del_one(td, key, value, flags|TDFLG_PARTIAL));
    while (1) {
      int r;
      switch (r = td_del_one(td, key, value, flags|TDFLG_PARTIAL)) {
      case 0: 
        continue;
      case TDERR_NOTFOUND:
        return 0;
      default:
        return r;
      }
    }
  } else {
    CHECK(td_del_one(td, key, value, flags));
  }
  return 0;
}

/**
 * \ingroup btree
 *
 * Deletes an item from the tree, optionally filling in the data
 * contained therein.
 *
 * \param td Pointer to an initialized td_t structure.
 * \param key Non-NULL Pointer to an initialized tdt_t structure,
 * containing the key.
 * \param value Pointer to an initialized tdt_t structure, which will be filled
 * with the removed value, or NULL if you're just interested in removal.
 * \param flags May be specified to retrieve the deleted value.
 *
 * \returns Zero on success, TDERR_NOTFOUND if the key cannot be found,
 * or any of the errors of the underlying functions.
 */
int td_del
  (td_t* td, const tdt_t* key, tdt_t* value, unsigned flags)
{
  int r;
  flags |= td->header.flags;
  CHECK(td_lock(td));
  r = td_del_locked(td, key, value, flags);
  CHECK(td_unlock(td));
  return r;
}

#ifdef __cplusplus
}
#endif
