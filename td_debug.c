/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "td_private.h"

static
int td_debug_empties
  (td_t* td)
{
  unsigned count = 0;
  unsigned ptr = td->header.off_e;
  while (ptr) {
    struct chunkhead chunkhead;
    CHECK(td_read_chunkhead(td, ptr, &chunkhead));
    fprintf(stderr, "Empty chunk @%d, next @%d, size %d\n",
      ptr, chunkhead.next, chunkhead.size
    );
    ptr = chunkhead.next;
    ++count;
  }
  if (td->header.nempties != count) {
    fprintf(stderr,
      "WARNING: Real number of empties is not consistent with header.\n"
    );
  }
  return 0;
}

/**
 * \ingroup btree
 * Debugs the structure and contents of a btree to stderr.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_debug
  (td_t* td)
{
  CHECK(td_read_header(td));
  fprintf(stderr,
    "TD: Size %u, top @%u, off_e @%u, nempties %u, "
    "nentries %u, nchunks %u\n",
    td->header.size, td->header.top, td->header.off_e,
    td->header.nempties, td->header.nentries, td->header.nchunks
  );
  CHECK(td_debug_empties(td));
  if (td->header.top == 0) {
    fprintf(stderr, "EMPTY\n");
  } else {
    CHECK(td_debug_node(td, td->header.top, 1));
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
