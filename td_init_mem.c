/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
int td_mem_read
  (td_t* td, unsigned off, void* buf, unsigned siz, void* ioarg)
{
  DISCARD_PARAMETER(ioarg);
  memcpy(buf, ((char*)(td->resource.mem)) + off, siz);
  return 0;
}

static
int td_mem_write
  (td_t* td, unsigned off, const void* buf, unsigned siz, void* ioarg)
{
  DISCARD_PARAMETER(ioarg);
  memcpy(((char*)(td->resource.mem)) + off, buf, siz);
  return 0;
}

/**
 * \ingroup btree
 * Initializes a btree to use a fixed piece of memory as resource.
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param flags Bits from the TDFLG_* values.
 * \param mem Start of the memory to use a resource.
 * \param size Size of the usable memory.
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_init_mem
  (td_t* td, unsigned flags, void* mem, unsigned size)
{
  if (flags & TDFLG_EXTEND) {
    RETURNERR(TDERR_INIT);
  }
  CHECK(td_init(td));
  td->resource.mem = mem;
  td->header.size = size;
  td->read = td_mem_read;
  td->write = td_mem_write;
  CHECK(td_init2(td, "tdi", 1, flags));
  return 0;
}

#ifdef __cplusplus
}
#endif
