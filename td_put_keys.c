/*
 * Copyright 2015 K.J. Hermans (kees@pink-frog.com)
 * This code is part of simpledbm, an API to a dbm on a finite resource.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#include "td_private.h"

static
int td_put_keys_for_value
  (td_t* td, unsigned valueoffset, unsigned flags, unsigned nkeys, va_list ap)
{
  unsigned i;

  for (i=0; i < nkeys; i++) {
    tdt_t* key = va_arg(ap, tdt_t*);
    CHECK(td_put_key(td, key, valueoffset, flags));
  }
  return 0;
}

static
int td_put_keys_locked
  (td_t* td, const tdt_t* value, unsigned flags, unsigned nkeys, va_list ap)
{
  int r;
  unsigned valueoffset;

  CHECK(td_store_value(td, value, nkeys, &valueoffset, flags));
  if ((r = td_put_keys_for_value(td, valueoffset, flags, nkeys, ap)) != 0) {
    CHECK(td_yield_all(td, valueoffset));
  }
  return r;
}

/**
 * Put a single value into the dbm under multiple keys.
 * 
 * --- Description ---
 *
 * \param td
 * \param value
 * \param flags
 * \param nkeys The number of keys presented to the
 * \param ... A list of tdt_t pointers, provided by the caller,
 * as long as nkeys.
 *
 * returns Zero on success, or non-zero on error.
 *
 * NB If you specify TDFLG_NODUPLKEYS, and only one of your keys
 * is duplicate, then the whole transaction will fail.
 */
int td_put_keys
  (td_t* td, const tdt_t* value, unsigned flags, unsigned nkeys, ...)
{
  int r;
  va_list ap;

  va_start(ap, nkeys);
  flags |= td->header.flags;
  CHECK(td_lock(td));
  r = td_put_keys_locked(td, value, flags, nkeys, ap);
  CHECK(td_unlock(td));
  va_end(ap);
  return r;
}

#ifdef __cplusplus
}
#endif
