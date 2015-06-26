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
int tdc_init_locked
  (td_t* td, tdc_t* tdc)
{
  memset(tdc, 0, sizeof(tdc_t));
  CHECK(td_read_header(td));
  tdc->td = td;
  tdc->scn = td->header.scn;
  if (td->header.top != 0) {
    tdc->path = TD_PATH_INIT(td->header.top);
    CHECK(td_iterate_to_min(td, &(tdc->path)));
  }
  return 0;
}

/**
 * \ingroup btree
 *
 * Initializes a cursor, moving it to the beginning of the tree.
 *
 * \param[in] td Non-NULL pointer to an initialized btree structure.
 * \param[out] tdc Non-NULL pointer to a potentially uninitialized cursor
 * structure.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int tdc_init
  (td_t* td, tdc_t* tdc)
{
  int r;
  CHECK(td_lock(td));
  r = tdc_init_locked(td, tdc);
  CHECK(td_unlock(td));
  return r;
}

#ifdef __cplusplus
}
#endif
