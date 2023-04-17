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
 * Commits all the changes made in the transaction to the main database.
 */
int tdx_commit
  (tdx_t* tdx)
{
  tdc_t cursor;
  tdt_t key, val;

  CHECK(tdc_init(&(tdx->changes), &cursor));
  while (1) {
    key.data = 0;
    key.size = 0;
    val.data = 0;
    val.size = 0;
    if (tdc_nxt(&cursor, &key, &val, TDFLG_ALLOCTDT) != 0) {
      break;
    }
    free(key.data);
    free(val.data);
  }
  CHECK(tdx_rollback(tdx));
  return 0;
}

#ifdef __cplusplus
}
#endif
