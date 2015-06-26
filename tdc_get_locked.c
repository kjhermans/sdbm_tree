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
 * Returns the element currently at the cursor from a locked btree resource.
 * \param tdc Non-NULL pointer to an initialized cursor structure.
 * \param key Non-NULL pointer to a potentially uninitialized tdt.
 * \param value Potentially NULL pointer to a potentially uninitialized tdt.
 * \param flags Bits from the TDCFLG_* values.
 * \returns Zero on success, or a TDERR_* value on error.
 */
int tdc_get_locked
  (tdc_t* tdc, tdt_t* key, tdt_t* value, unsigned flags)
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
  if (value) {
    CHECK(
      td_read_value(
        tdc->td,
        tdc->path.head->keyhead.value,
        value,
        flags
      )
    );
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
