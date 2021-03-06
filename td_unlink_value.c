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
 * Unlinks a value, decreasing the refcount on it, and removing if 0.
 *
 * ---- Description ----
 *
 * \param td Non-NULL pointer to initialized td_t structure.
 * \param valueptr Pointer to the value node.
 *
 * \returns Zero on success, or non-zero on error.
 */
int td_unlink_value
  (td_t* td, unsigned valueptr)
{
  struct chunkhead chunkheader;
  CHECK(td_read_chunkhead(td, valueptr, &chunkheader));
  switch (chunkheader.refcount) {
  case 0:
    fprintf(stderr, "WARNING: Dangling value node found at %u\n", valueptr);
    __attribute__ ((fallthrough));
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
