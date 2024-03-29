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
 * Puts a name/value inside a transaction context into the db.
 */
int tdx_put
  (tdx_t* tdx, const tdt_t* key, const tdt_t* value, unsigned flags)
{
  unsigned char type = TDX_PUT;
  tdt_t valuevec[] = {
    {
      value->data,
      value->size
    },
    {
      &type,
      1
    }
  };

  CHECK(td_put_vec(&(tdx->changes), key, valuevec, 2, flags));
  return 0;
}

#ifdef __cplusplus
}
#endif
