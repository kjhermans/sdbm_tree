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
 * \ingroup btree
 *
 * Initializes a tdt.
 *
 * \param tdt
 * \param mem
 * \param size
 *
 * \returns An initialized tdt.
 */
tdt_t tdt_init(tdt_t* tdt, void* mem, unsigned size) {
  if (tdt == NULL) {
    return (tdt_t){ mem, size };
  } else {
    tdt->data = mem;
    tdt->size = size;
    return *tdt;
  }
}

#ifdef __cplusplus
}
#endif
