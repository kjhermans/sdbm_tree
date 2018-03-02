/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
int td_get_stream_locked
  (td_t* td, const tdt_t* key, int fd, unsigned flags)
{
  struct path path;
  CHECK(td_read_header(td));
  if (td->header.top == 0) {
    return TDERR_NOTFOUND;
  }
  path = TD_PATH_INIT(td->header.top);
  CHECK(td_iterate(td, &path, key, 0));
  if (path.head->cmp == 0) {
    if (fd >= 0) {
      CHECK(td_read_value_stream(td, path.head->keyhead.value, fd, flags));
    }
    return 0;
  }
  return TDERR_NOTFOUND;
}

/**
 * \ingroup btree
 *
 * Gets a key/value pair from a btree as a stream.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param key Non-NULL pointer to a potentially uninitialized tdt.
 * \param fd File descriptor to which the value will be written.
 * \param flags Bits from the TDCFLG_* values.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_get_stream
  (td_t* td, const tdt_t* key, int fd, unsigned flags)
{
  int r;
  flags |= td->header.flags;
  CHECK(td_lock(td));
  r = td_get_stream_locked(td, key, fd, flags);
  CHECK(td_unlock(td));
  return r;
}

#ifdef __cplusplus
}
#endif
