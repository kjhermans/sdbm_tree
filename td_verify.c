/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
int td_verify_valuenode
  (td_t* td, unsigned offset, int fix)
{
  struct chunkhead chunkhead;

  DISCARD_PARAMETER(fix);
  while (offset) {
    CHECK(td_read_chunkhead(td, offset, &chunkhead));
    {
      unsigned chunkdatasize = chunkhead.size - sizeof(chunkhead);
      uchar buf[ chunkdatasize ];
      CHECK(td_read(td, offset + sizeof(chunkhead), buf, chunkdatasize));
      if (td->header.flags & TDFLG_CHECKSUM) {
        CHECK(td_checksum_verify(buf, chunkdatasize, chunkhead.checksum));
      }
    }
    offset = chunkhead.next;
  }
  return 0;
}

static
int td_verify_keynode
  (td_t* td, unsigned offset, int fix)
{
  struct keyhead keyhead;
  CHECK(td_read_keyhead(td, offset, &keyhead));
  {
    uchar buf[ keyhead.size ];
    tdt_t key = { buf, sizeof(buf) };
    CHECK(td_read_keydata(td, offset, &keyhead, &key, 0));
  }
  if (keyhead.previous) {
    CHECK(td_verify_keynode(td, keyhead.previous, fix));
  }
  if (keyhead.next) {
    CHECK(td_verify_keynode(td, keyhead.next, fix));
  }
  CHECK(td_verify_valuenode(td, keyhead.value, fix));
  return 0;
}

/**
 * Verifies the content of the btree resource.
 * Should this function return a fatal error, the btree should not be used.
 *
 * \param td The btree to be verified.
 * \param fix Whether or not to fix minor errors.
 *
 * \par NB
 * The fix function is unimplemented.
 */
int td_verify
  (td_t* td, int fix)
{
  if (td->header.top) {
    CHECK(td_verify_keynode(td, td->header.top, fix));
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
