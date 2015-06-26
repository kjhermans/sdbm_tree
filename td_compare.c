/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/**
 * \ingroup btree_private
 * In-house comparison call-back function.
 * Compares to keys, potentially partially.
 * If not partially, compares like memcmp().
 * If partially, only until the length of key1.
 *
 * \param td Non-NULL pointer to an initialized btree structure. 
 * \param key1 Non-NULL pointer to an initialized tdt.
 * \param key2 Non-NULL pointer to an initialized tdt.
 * \param partial Boolean. Whether or not partial matches are allowed.
 * \param arg Callback API user-provided argument.
 * (Unused in this implementation).
 *
 * \returns Zero when (partially) equal, smaller than zero when key1 is
 * considered smaller than key2, greater than zero when key1 is considered
 * greater than key2.
 */
int td_compare
  (td_t* td, const tdt_t* key1, const tdt_t* key2, int partial, void* arg)
{
  DISCARD_PARAMETER(td);
  DISCARD_PARAMETER(arg);
  if (key1->size < key2->size) {
    if (partial) {
      return memcmp(key1->data, key2->data, key1->size);
    } else {
      int r;
      if ((r = memcmp(key1->data, key2->data, key1->size)) == 0) {
        return -1;
      } else {
        return r;
      }
    }
  } else if (key1->size > key2->size) {
    int r;
    if ((r = memcmp(key1->data, key2->data, key2->size)) == 0) {
      return 1;
    } else {
      return r;
    }
  } else {
    return memcmp(key1->data, key2->data, key1->size);
  }
}

#ifdef __cplusplus
}
#endif
