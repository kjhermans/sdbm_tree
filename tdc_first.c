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
int tdc_first_locked
  (tdc_t* tdc)
{
  memset(&(tdc->path), 0, sizeof(struct path));
  TD_PATH_INIT(tdc->td->header.top);
  CHECK(td_iterate_to_min(tdc->td, &(tdc->path)));
  tdc->scn = tdc->td->header.scn;
  return 0;
}

/**
 * \ingroup btree
 *
 * Moves the cursor to the leftmost element.
 *
 * \param tdc Non-NULL pointer to an initialized cursor structure.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int tdc_first
  (tdc_t* tdc)
{
  int r;
  CHECK(td_lock(tdc->td));
  r = tdc_first_locked(tdc);
  CHECK(td_unlock(tdc->td));
  return r;
}

#ifdef __cplusplus
}
#endif
