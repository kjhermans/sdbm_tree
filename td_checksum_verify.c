/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
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
 * Verifies a checksum of a piece of memory.
 *
 * \param[in] mem Non-NULL pointer to the beginning of the memory
 * to be checksummed.
 * \param[in] length Size of the memory to be checksummed.
 * \param[in] checksum The checksum to verify.
 *
 * \returns Zero when the piece of memory is verified by the checksum.
 * TDERR_CHECKSUM on failure.
 */
int td_checksum_verify
  (void* mem, unsigned length, unsigned checksum)
{
  unsigned control;
  td_checksum_create(mem, length, &control);
  if (checksum == control) {
    return 0;
  } else {
    RETURNERR(TDERR_CHECKSUM);
  }
}

#ifdef __cplusplus
}
#endif
