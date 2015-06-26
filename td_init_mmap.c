/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/mman.h>

#include "td_private.h"

static
int td_mmap_read
  (td_t* td, unsigned off, void* buf, unsigned siz, void* ioarg)
{
  DISCARD_PARAMETER(ioarg);
  memcpy(buf, ((char*)(td->resource.mem)) + off, siz);
  return 0;
}

static
int td_mmap_write
  (td_t* td, unsigned off, const void* buf, unsigned siz, void* ioarg)
{
  DISCARD_PARAMETER(ioarg);
  memcpy(((char*)(td->resource.mem)) + off, buf, siz);
  msync(td->resource.mem + off, siz, MS_SYNC);
  return 0;
}

static
void td_mmap_close
  (td_t* td)
{
  munmap(td->resource.mem, td->header.size);
}

/**
 * \ingroup btree
 * Initializes a btree to use a memory-mapped file as a resource.
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param flags Bits from the TDFLG_* values.
 * \param fd Filedescriptor to be memory mapped
 * \param off
 * \param siz
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_init_mmap
  (td_t* td, unsigned flags, int fd, unsigned off, unsigned siz)
{
  if (flags & TDFLG_EXTEND) {
    RETURNERR(TDERR_INIT);
  }
  CHECK(td_init(td));
  if ((td->resource.mem =
       mmap(0, siz, PROT_READ|PROT_WRITE, MAP_SHARED, fd, off)) == 0) {
    RETURNERR(TDERR_INIT);
  }
  td->resource.offset = off;
  td->header.size = siz;
  td->read = td_mmap_read;
  td->write = td_mmap_write;
  td->close = td_mmap_close;
  CHECK(td_init2(td, "tdi", 1, flags));
  return 0;
}

#ifdef __cplusplus
}
#endif
