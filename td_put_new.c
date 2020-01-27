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
 * \ingroup btree_private
 *
 * Puts a new node (with a new key) into the btree.
 *
 * ---- Description ----
 *
 * \param td Non-NULL pointer to an initialized btree structre.
 * \param key Non-NULL pointer to an initialized tdt_t structure.
 * \param valueptr Offset to the written value node.
 * \param keyptr On successful return, the offset of the written key node.
 * \param previous Offset to the previous node.
 * \param next Offset to the next node.
 * \param flags Bits from the TDFLG_* values.
 *
 * \returns Zero on success, or any error of the underlying functions.
 */
int td_put_new
  (
    td_t*         td,
    const tdt_t*  key,
    unsigned      valueptr,
    unsigned*     keyptr,
    unsigned      previous,
    unsigned      next,
    unsigned      flags
  )
{
  struct keyhead keyhead = TD_KEYHEAD_NULL;
  keyhead.value = valueptr;
  keyhead.size = sizeof(struct keyhead) + key->size;
  keyhead.previous = previous;
  keyhead.next = next;
  if (flags & TDFLG_CHECKSUM) {
    td_checksum_create(key->data, key->size, &(keyhead.checksum));
  }
  CHECK(td_claim(td, 1, keyptr, &(keyhead.size)));
  CHECK(td_write_keyhead(td, (*keyptr), &keyhead));
  CHECK(td_write(td, (*keyptr) + sizeof(struct keyhead), key->data, key->size));
  ++(td->header.nentries);
  ++(td->header.scn);
  return 0;
}

#ifdef __cplusplus
}
#endif
