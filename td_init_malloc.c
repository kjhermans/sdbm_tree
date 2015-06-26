/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "td_private.h"

static
int td_malloc_read
  (td_t* td, unsigned off, void* buf, unsigned siz, void* ioarg)
{
  DISCARD_PARAMETER(ioarg);
  memcpy(buf, ((char*)(td->resource.mem)) + off, siz);
  return 0;
}

static
int td_malloc_write
  (td_t* td, unsigned off, const void* buf, unsigned siz, void* ioarg)
{
  DISCARD_PARAMETER(ioarg);
  memcpy(((char*)(td->resource.mem)) + off, buf, siz);
  return 0;
}

static
unsigned td_malloc_extend
  (td_t* td, unsigned d, void* arg)
{
  DISCARD_PARAMETER(arg);
  void* newmem = realloc(td->resource.mem, td->header.size + d);
  if (newmem) {
    td->resource.mem = newmem;
    return d;
  }
  return 0;
}

static
void* td_malloc_realloc
  (td_t* td, void* ptr, unsigned size, void* arg)
{
  DISCARD_PARAMETER(td);
  DISCARD_PARAMETER(arg);
  return realloc(ptr, size);
}

/**
 * \ingroup btree
 * Initializes a btree to use a malloc'ed piece of memory as resource.
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param flags Bits from the TDFLG_* values.
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_init_malloc
  (td_t* td, unsigned flags)
{
  CHECK(td_init(td));
  td->header.size = 4 * 1024;
  if ((td->resource.mem = malloc(td->header.size)) == NULL) {
    RETURNERR(TDERR_INIT);
  }
  td->read = td_malloc_read;
  td->write = td_malloc_write;
  if (flags & TDFLG_EXTEND) {
    td->extend = td_malloc_extend;
  }
  if (flags & TDFLG_ALLOCTDT) {
    td->realloc = td_malloc_realloc;
  }
  CHECK(td_init2(td, "tdi", 1, flags));
  return 0;
}

#ifdef __cplusplus
}
#endif
