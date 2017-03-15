/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

#define FNV_OFFSET_BASIS 0xcbf29ce484222325LL
#define FNV_PRIME 0x100000001b3LL

/**
 * \ingroup btree_private
 *
 * Creates a checksum of a piece of memory.
 * Does this using the FNV-1a algorithm, which is 64-bit, and
 * must be reduced to sizeof(unsigned). So it is a variety of FNV.
 *
 * \code

   hash = FNV_offset_basis
   for each byte_of_data to be hashed
        hash = hash XOR byte_of_data
        hash = hash × FNV_prime
   return hash

 * \endcode
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
  u_int64_t workingmem = FNV_OFFSET_BASIS;

  for (i=0; i < length; i++) {
    workingmem = workingmem ^ _mem[ i ];
    workingmem = workingmem * FNV_PRIME;
  }
  (*checksum) = (unsigned)workingmem;
}

#ifdef __cplusplus
}
#endif
