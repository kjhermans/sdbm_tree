/*
** Copyright 2015 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/**
 * \ingroup btree
 *
 * Returns the element currently at the cursor.
 * Does exactly what tdc_prv() and tdc_nxt() do, minus the moving.
 *
 * \param tdc Non-NULL pointer to an initialized cursor structure.
 * \param key Potentially NULL pointer to a potentially uninitialized tdt.
 * \param value Potentially NULL pointer to a potentially uninitialized tdt.
 * \param flags Bits from the TDCFLG_* values.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 *
 * NOTE: A current limitation of this library is that the cursor will
 * become invalid when doing mutations on the database while iterating
 * using a cursor. This is because the cursor stores the path to the
 * current node, and paths may change as a result of re-balancing of
 * the tree when doing mutations. This means that tdc_nxt(), tdc_prv(),
 * tdc_get() and tdc_rpl() may all return non-zero values for perhaps
 * a non-obvious reason.
 *
 */
int tdc_get
  (tdc_t* tdc, tdt_t* key, tdt_t* value, unsigned flags)
{
  int r;
  CHECK(td_lock(tdc->td));
  r = tdc_get_locked(tdc, key, value, flags);
  CHECK(td_unlock(tdc->td));
  return r;
}

#ifdef __cplusplus
}
#endif
