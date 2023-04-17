/*
** Copyright 2021 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a btree on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

#include "td_private.h"

/**
 * Ingnores all the changes made in the transaction.
 */
int tdx_rollback
  (tdx_t* tdx)
{
  tdx->changes.close(&(tdx->changes));
  unlink(tdx->path);
  memset(tdx, 0, sizeof(*tdx));
  return 0;
}

#ifdef __cplusplus
}
#endif
