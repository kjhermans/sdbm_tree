/*
** Copyright 2021 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a btree on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "td_private.h"

static
unsigned tdx_id = 0;

/**
 * Initializes a transaction context.
 * A transaction context takes the place of your td_t pointer.
 */
int tdx_init
  (td_t* td, tdx_t* tdx)
{
  memset(tdx, 0, sizeof(*tdx));
  tdx->orig = td;
  tdx->id = tdx_id++;
  snprintf(tdx->path, sizeof(tdx->path),
    "/tmp/txn.%d.%u.db", getpid(), tdx->id
  );
  CHECK(td_open(&(tdx->changes), tdx->path, 0, O_RDWR, 0640));
  return 0;
}

#ifdef __cplusplus
}
#endif
