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
int tdc_get_stream_locked
  (tdc_t* tdc, tdt_t* key, int fd, unsigned flags)
{
  CHECK(td_read_header(tdc->td));
  if (tdc->scn != tdc->td->header.scn) {
    RETURNERR(TDERR_INVAL);
  }
  if (tdc->path.size == 0 || tdc->path.head == 0) {
    return TDERR_NOTFOUND;
  }
  if (tdc->path.head->cmp != 0) {
    RETURNERR(TDERR_STRUCT);
  }
  if (key) {
    CHECK(
      td_read_keydata(
        tdc->td,
        tdc->path.head->ptr,
        &(tdc->path.head->keyhead),
        key,
        flags
      )
    );
  }
  if (fd >= 0) {
    CHECK(
      td_read_value_stream(
        tdc->td,
        tdc->path.head->keyhead.value,
        fd,
        flags
      )
    );
  }
  return 0;
}

/**
 * \ingroup btree
 *
 * Returns the element currently at the cursor.
 * Does exactly what tdc_prv() and tdc_nxt() do, minus the moving.
 *
 * \param tdc Non-NULL pointer to an initialized cursor structure.
 * \param key Potentially NULL pointer to a potentially uninitialized tdt.
 * \param fd File descriptor to write value to.
 * \param flags Bits from the TDCFLG_* values.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int tdc_get_stream
  (tdc_t* tdc, tdt_t* key, int fd, unsigned flags)
{
  int r;
  CHECK(td_lock(tdc->td));
  r = tdc_get_stream_locked(tdc, key, fd, flags);
  CHECK(td_unlock(tdc->td));
  return r;
}

#ifdef __cplusplus
}
#endif
