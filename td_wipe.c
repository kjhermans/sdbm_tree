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
int td_wipe_locked
  (td_t* td)
{
  if (td->header.size >= sizeof(struct td_header) + sizeof(struct chunkhead)) {
    unsigned e_off = sizeof(struct td_header);
    unsigned e_size = td->header.size - e_off;
    struct chunkhead empty = { 0, e_size, 0, 0 };
    td->header.off_e = e_off;
    td->header.scn = 0;
    td->header.nentries = 0;
    td->header.nempties = 1;
    td->header.nchunks = 1;
    CHECK(td_write_chunkhead(td, e_off, &empty));
    if (td->header.flags & TDFLG_WIPEDELETED) {
      CHECK(td_yield_set_zero(td, e_off));
    }
  } else {
    td->header.off_e = 0;
    td->header.scn = 0;
    td->header.nentries = 0;
    td->header.nempties = 0;
    td->header.nchunks = 0;
  }
  CHECK(td_write_header(td));
  return 0;
}

/**
 * \ingroup btree
 *
 * Wipes all data from a btree by creating one big single empty chunk
 * from all available space beyond the header.
 *
 * \param td Non-NULL pointer to initialized td_t structure.
 *
 * \returns Zero on success, or non-zero on error.
 */
int td_wipe
  (td_t* td)
{
  int r;
  CHECK(td_lock(td));
  r = td_wipe_locked(td);
  CHECK(td_unlock(td));
  return r;
}

#ifdef __cplusplus
}
#endif
