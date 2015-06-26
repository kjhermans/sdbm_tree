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
 * Creates a checksum of a piece of memory.
 *
 * \param[in] mem Non-NULL pointer to the beginning of the memory
 * to be checksummed.
 * \param[in] length Size of the memory to be checksummed.
 * \param[out] checksum On return, contains the checksum of the memory piece.
 */
void td_checksum_create
  (void* mem, unsigned length, unsigned* checksum)
{
  uchar* _mem = (uchar*)mem;
  unsigned i;
  (*checksum) = 0;
  for (i=0; i < length; i++) {
    (*checksum) += ( _mem[ i ] * ((i * 2) + 1));
  }
}

#ifdef __cplusplus
}
#endif
