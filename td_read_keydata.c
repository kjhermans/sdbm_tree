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
 * Reads the key contents into a tdt.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param[in] keyoff Offset of the key header.
 * \param[in] keyhead Key header structure, describing the size of the key.
 * \param[out] key On successful return, contains the key.
 * \param[in] flags Bits from TDFLG_* values.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_read_keydata
  (
    td_t* td,
    unsigned keyoff,
    struct keyhead* keyhead,
    tdt_t* key,
    unsigned flags
  )
{
  if (keyoff < sizeof(struct td_header)) {
    RETURNERR(TDERR_BOUNDS);
  }
  flags |= td->header.flags;
  int reallocating = (
    key->data == 0 &&
    key->size == 0 &&
    (flags & TDFLG_ALLOCTDT) &&
    td->realloc != 0
  );
  unsigned headsize = sizeof(struct keyhead);
  unsigned keysize = keyhead->size - headsize;
  if (reallocating) {
    void* mem = td->realloc(td, key->data, keysize, td->reallocarg);
    if (mem) {
      key->data = mem;
      key->size = keysize;
    } else {
      RETURNERR(TDERR_NOMEM);
    }
  } else {
    if (keysize > key->size) {
      keysize = key->size;
    } else {
      key->size = keysize;
    }
  }
  CHECK(td_read(td, keyoff + headsize, key->data, keysize));
  if (flags & TDFLG_CHECKSUM && keysize == keyhead->size - headsize) {
    CHECK(td_checksum_verify(key->data, keysize, keyhead->checksum));
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
