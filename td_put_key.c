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
 * Puts a value in the db and its (single) key.
 * 
 * ---- Description ----
 *
 * \param td
 * \param key
 * \param value
 * \param flags
 *
 * \returns
 */
int td_put_key
  (td_t* td, const tdt_t* key, const unsigned valueptr, unsigned flags)
{
  if (td->header.top == 0) {
    unsigned ptr;
    CHECK(td_put_new(td, key, valueptr, &ptr, 0, 0, flags));
    td->header.top = ptr;
  } else {
    struct path path = TD_PATH_INIT(td->header.top);
    CHECK(td_iterate(td, &path, key, 0));
    if (path.head->cmp == 0) {
      if (flags & TDFLG_DUPLKEYS) {
        unsigned ptr;
        if (path.head->keyhead.previous == 0) {
          CHECK(td_put_new(td, key, valueptr, &ptr, 0, 0, flags));
          path.head->keyhead.previous = ptr;
        } else {
          CHECK(
            td_put_new(
              td,
              key,
              valueptr,
              &ptr,
              0,
              path.head->keyhead.next,
              flags
            )
          );
          path.head->keyhead.next = ptr;
        }
        CHECK(td_write_keyhead(td, path.head->ptr, &(path.head->keyhead)));
      } else if (flags & TDFLG_NOOVERWRITE) {
        RETURNERR(TDERR_DUP);
      } else {
        CHECK(
          td_put_replace(
            td,
            valueptr,
            path.head->ptr,
            &(path.head->keyhead)
          )
        );
      }
    } else if (path.head->cmp < 0) {
      unsigned ptr;
      CHECK(td_put_new(td, key, valueptr, &ptr, 0, 0, flags));
      path.head->keyhead.previous = ptr;
      CHECK(td_write_keyhead(td, path.head->ptr, &(path.head->keyhead)));
    } else {
      unsigned ptr;
      CHECK(td_put_new(td, key, valueptr, &ptr, 0, 0, flags));
      path.head->keyhead.next = ptr;
      CHECK(td_write_keyhead(td, path.head->ptr, &(path.head->keyhead)));
    }
  }
  CHECK(td_write_header(td));
  CHECK(td_rebalance(td));
  return 0;
}

#ifdef __cplusplus
}
#endif
