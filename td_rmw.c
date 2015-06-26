/*
 * Copyright 2015 K.J. Hermans (kees@pink-frog.com)
 * This code is part of simpledbm, an API to a dbm on a finite resource.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
int td_rmw_locked
  (
    td_t* td,
    tdt_t* key,
    tdt_t* value,
    unsigned flags,
    rmwfnc_t callback,
    void* arg
  )
{
  struct path path;

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
        CHECK(callback(td, key, value, arg));
        CHECK(
          td_put_replace(
            td,
            path.head->keyhead.value,
            path.head->ptr,
            &(path.head->keyhead)
          )
        );
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
 */
int td_rmw
  (
    td_t* td,
    tdt_t* key,
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
