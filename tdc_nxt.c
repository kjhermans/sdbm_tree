/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
int tdc_nxt_locked
  (tdc_t* tdc, tdt_t* key, tdt_t* value, unsigned flags)
{
  int r;

  CHECK(tdc_get_locked(tdc, key, value, flags));
  switch (r = td_path_next(tdc->td, &(tdc->path))) {
  case 0:
    return 0;
  case TDERR_NOTFOUND:
    tdc->path.size = 0;
    tdc->path.head = 0;
    return 0;
  default:
    return r;
  }
}

/**
 * \ingroup btree
 *
 * Retrieves the element at the cursor, and subsequently moves the cursor
 * to the next element.
 *
 * \param tdc Non-NULL pointer to an initialized cursor structure.
 * \param key Potentially NULL pointer to a potentially uninitialized tdt.
 * \param value Potentially NULL pointer to a potentially uninitialized tdt.
 * \param flags Bits from the TDCFLG_* values.
 *
 * \returns Zero on success, TDERR_NOTFOUND at the end of the btree,
 * or another TDERR_* value on error.
 */
int tdc_nxt
  (tdc_t* tdc, tdt_t* key, tdt_t* value, unsigned flags)
{
  int r;
  CHECK(td_lock(tdc->td));
  r = tdc_nxt_locked(tdc, key, value, flags);
  CHECK(td_unlock(tdc->td));
  return r;
}

#ifdef __cplusplus
}
#endif
