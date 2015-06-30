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
int tdc_mov_locked
  (tdc_t* tdc, const tdt_t* key, unsigned flags)
{
  td_t* td = tdc->td;
  CHECK(td_read_header(td));
  if ((tdc->path.stack[ 0 ].ptr = tdc->td->header.top) == 0) {
    return TDERR_NOTFOUND;
  }
  tdc->path.size = 1;
  if (tdc->scn != td->header.scn) {
    RETURNERR(TDERR_INVAL);
  }
  CHECK(td_iterate(td, &(tdc->path), key, (flags & TDCFLG_PARTIAL)));
  if (tdc->path.head->cmp == 0) {
    return 0;
  } else if (flags & TDCFLG_EXACT) {
    tdc->path.size = 0;
    tdc->path.head = 0;
    return TDERR_NOTFOUND;
  } else {
    tdc->path.head->cmp = 0;
    return 0;
  }
}

/**
 * \ingroup btree
 *
 * Moves the cursor to a point at or near a given key.
 *
 * There are two modes of calling this function:
 * - When a key is given, moves the cursor to the place AT, or JUST BEFORE the
 *   key, depending on the bits set in the 'flags' parameter.  Also renews the
 *   System Change Number cache in the cursor, renewing the cursor if it had
 *   become stale.
 * - If a key isn't given, one the TDFLG_BEGIN or TDFLG_END bits must be
 *   set inside 'flags' parameter.  The cursor will then be set at the
 *   beginning or end of the btree, respectively.
 *
 * \param tdc Pointer to an initialized tdc_t structure, potentially stale.
 * \param key Pointer to an initialized dbt_t structure, containing a key.
 * \param flags Flags; OR-ed together TDFLG_PARTIAL or TDFLG_EXACT, or zero,
 * when a key is given, or one of TDFLG_BEGIN or TDFLG_END when it is not.
 *
 * \returns Zero on success, TDERR_NOTFOUND if the key cannot be found exactly
 * and the TDFLG_EXACT flag is set, or any of the errors of the underlying
 * functions.
 *
 * Bear in mind that a partial key match is considered an
 * exact match when TDFLG_PARTIAL has been set during initialization.
 * \par Flags and matches:
 * Since the terminology of 'partial' and 'exact' matches may be confusing,
 * let's try to clear it up a bit more.
 *
 * \par
 * A partial match is a match using a search key that can be considered
 * to be a part of the tested key; under normal circumstances
 * (that is, without a customized 'compare' callback function) this means
 * that the search key, up to its own size, contains the same bytes as the
 * tested key (although the tested key may be longer).  A partial match is
 * then passed from the search algorithm to the function-specific algorithm
 * (get, put, delete, move etc.) as an exact match.  That function, in its
 * turn, decides on whether it needs an exact match or not, and proceeds to
 * run with it.  In the case of tdc_mov(), this is left up to the caller.
 * The following paragraph provides a diagram of this behaviour.
 *
 * \par
 * Given the list [ 'aaa', 'bbb', 'ccc' ], and the search keys
 * 'bbb' (perfect match), 'bb' (partial match) and 'aab' (no match),
 * the cursor is moved to the following locations, given all TDFLG_EXACT
 * and TDFLG_PARTIAL flag combinations:
 * - Search 'bbb' with no flags; cursor at 'bbb'
 * - Search 'bbb' with TDFLG_PARTIAL; cursor at 'bbb'
 * - Search 'bbb' with TDFLG_PARTIAL|TDFLG_EXACT; cursor at 'bbb'
 * - Search 'bbb' with TDFLG_EXACT; cursor at 'bbb'
 * - Search 'bb' with no flags; cursor at 'aaa'
 * - Search 'bb' with TDFLG_PARTIAL; cursor at 'bbb'
 * - Search 'bb' with TDFLG_PARTIAL|TDFLG_EXACT; cursor at 'bbb'
 * - Search 'bb' with TDFLG_EXACT; returns TDERR_NOTFOUND
 * - Search 'aab' with no flags; cursor at 'aaa'
 * - Search 'aab' with TDFLG_PARTIAL; cursor at 'aaa'
 * - Search 'aab' with TDFLG_PARTIAL|TDFLG_EXACT; returns TDERR_NOTFOUND
 * - Search 'aab' with TDFLG_EXACT; returns TDERR_NOTFOUND
 *
 * \par Using cursors in code:
 * Unless you have only specified the TDFLG_EXACT flag (and not the
 * TDFLG_PARTIAL flag, and verified the return value of this
 * function to be zero, of course), you should always check where you are
 * by using on of the key-filling cursor related functions, such as
 * tdc_get(), tdc_nxt() or tdc_itr().
 */
int tdc_mov
  (tdc_t* tdc, const tdt_t* key, unsigned flags)
{
  int r;
  flags |= tdc->td->header.flags;
  CHECK(td_lock(tdc->td));
  r = tdc_mov_locked(tdc, key, flags);
  CHECK(td_lock(tdc->td));
  return r;
}

#ifdef __cplusplus
}
#endif
