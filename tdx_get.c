/*
** Copyright 2021 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a btree on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/**
 * Get a value in a transaction context.
 * This is done by making a local copy in the changes db, which will
 * make a subsequent get of the same key cheap. However, the first
 * get in a transaction context is not cheap.
 */
int tdx_get
  (tdx_t* tdx, const tdt_t* key, tdt_t* value, unsigned flags)
{
  int r;
  unsigned char* valuedata = value->data;

  switch (r = td_get(&(tdx->changes), key, value, flags)) {
  case 0:
    if (value->size == 0) {
      return TDERR_STRUCT;
    }
    switch (valuedata[ value->size-1 ]) {
    case TDX_PUT:
      value->size -= 1;
      return 0;
    case TDX_DEL:
      return TDERR_NOTFOUND;
    default:
      return TDERR_STRUCT;
    }
  case TDERR_NOTFOUND:
    CHECK(td_get(tdx->orig, key, value, flags));
    return 0;
  default:
    return r;
  }
}

#ifdef __cplusplus
}
#endif
