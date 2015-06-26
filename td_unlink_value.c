/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/**
 * Unlinks a value, decreasing the refcount on it, and removing if 0.
 *
 * ---- Description ----
 *
 * \param td
 * \param valueptr
 *
 * \returns
 */
int td_unlink_value
  (td_t* td, unsigned valueptr)
{
  struct chunkhead chunkheader;
  CHECK(td_read_chunkhead(td, valueptr, &chunkheader));
  switch (chunkheader.refcount) {
  case 0:
    fprintf(stderr, "WARNING: Dangling value node found at %u\n", valueptr);
  case 1:
    CHECK(td_yield_all(td, valueptr));
    break;
  default:
    --(chunkheader.refcount);
    CHECK(td_write_chunkhead(td, valueptr, &chunkheader));
    break;
  }
  return 0;
}

#ifdef __cplusplus
extern "C" {
#endif
