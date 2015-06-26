/*
 * Copyright 2015 K.J. Hermans (kees@pink-frog.com)
 * This code is part of simpledbm, an API to a dbm on a finite resource.
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
int td_put_locked_key
  (td_t* td, const tdt_t* key, const tdt_t* value, unsigned flags)
{
  int r;
  unsigned valueptr;

  CHECK(td_read_header(td));
  CHECK(td_store_value(td, value, 1, &valueptr, flags));
  if ((r = td_put_key(td, key, valueptr, flags)) != 0) {
    CHECK(td_yield_all(td, valueptr));
  }
  return r;
}

#ifdef __cplusplus
}
#endif
