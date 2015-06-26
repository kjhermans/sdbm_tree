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
int tdc_rpl_locked
  (tdc_t* tdc, const tdt_t* value, unsigned flags)
{
  DISCARD_PARAMETER(flags);
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
  if (value) {
    CHECK(
      td_put_replace(
        tdc->td,
        tdc->path.head->keyhead.value,
        tdc->path.head->ptr,
        &(tdc->path.head->keyhead)
      )
    );
  }
  return 0;
}

/**
 * \ingroup btree
 *
 * Replaces the value of the element currently at the cursor.
 *
 * \param tdc Non-NULL pointer to an initialized cursor structure.
 * \param value Potentially NULL pointer to a potentially uninitialized tdt.
 * \param flags Bits from TDFLG_* and TDCFLG_* values.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int tdc_rpl
  (tdc_t* tdc, const tdt_t* value, unsigned flags)
{
  int r;
  flags |= tdc->td->header.flags;
  CHECK(td_lock(tdc->td));
  r = tdc_rpl_locked(tdc, value, flags);
  CHECK(td_unlock(tdc->td));
  return r;
}
