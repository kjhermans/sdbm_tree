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
int td_rmw_locked
  (
    td_t* td,
    const tdt_t* key,
    tdt_t* value,
    unsigned flags,
    rmwfnc_t callback,
    void* arg
  )
{
  struct path path;
  unsigned valueptr;

  CHECK(td_read_header(td));
  if (td->header.top == 0) {
    return TDERR_NOTFOUND;
  }
  path = TD_PATH_INIT(td->header.top);
  CHECK(td_iterate(td, &path, key, 0));
  if (path.head->cmp == 0) {
    if (value) {
      CHECK(td_read_value(td, path.head->keyhead.value, value, flags));
      if (callback) {
        int r;
        switch (r = callback(td, key, value, arg)) {
        case 0:
          CHECK(td_store_value(td, value, 1, &valueptr, flags));
          CHECK(
            td_put_replace(
              td,
              valueptr,
              path.head->ptr,
              &(path.head->keyhead)
            )
          );
          __attribute__ ((fallthrough));
        case TDERR_NOTFOUND:
          CHECK(td_write_header(td));
          return 0;
        default:
          return r;
        }
      }
    }
    return 0;
  }
  return TDERR_NOTFOUND;
}

/**
 * \ingroup btree
 *
 * Implements read-modify-write ('RMW') for this btree API.
 *
 * The idea behind RMW is that both node-examination and -manipulation
 * can take place inside the same lock, allowing for certain data
 * transactions to happen.
 * The function caller provides a callback function as argument,
 * which gets called as soon as the btree is locked and the key is found.
 * Inside the * callback, the caller implements changes to the value tdt
 * and returns zero. The renewed value is then stored and the lock
 * is cleared.
 *
 * \param td Non-NULL pointer to initialized td_t structure.
 * \param key Non-NULL pointer to initialized tdt_t structure.
 * \param value Non-NULL pointer to initialized tdt_t structure.
 * \param flags Bits from TDFLG_* values.
 * \param callback The callback that examines the value, potentially
 * changes it, and returns zero to have it stored. Should any such function
 * return TDERR_NOTFOUND, then the value is considered unchanged, and
 * zero is returned by the td_rmw() function. Any other error is passed
 * along to the caller of td_rmw().
 * \param arg Any pointer, also NULL, that the caller wants passed to
 * the callback.
 *
 * \returns Zero on success, or non-zero on error.
 */
int td_rmw
  (
    td_t* td,
    const tdt_t* key,
    tdt_t* value,
    unsigned flags,
    rmwfnc_t callback,
    void* arg
  )
{
  int r;
  flags |= td->header.flags;
  CHECK(td_lock(td));
  r = td_rmw_locked(td, key, value, flags, callback, arg);
  CHECK(td_unlock(td));
  return r;
}

#ifdef __cplusplus
}
#endif
