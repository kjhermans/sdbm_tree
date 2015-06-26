/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>

#include "td_private.h"

static
int td_chunk_lock
  (td_t* td, int lock, void* lockarg)
{
  DISCARD_PARAMETER(lockarg);
  if (lock) {
    return flock(td->resource.fd, LOCK_EX);
  } else {
    return flock(td->resource.fd, LOCK_UN);
  }
}

static
int td_chunk_read
  (td_t* td, unsigned off, void* buf, unsigned siz, void* ioarg)
{
  DISCARD_PARAMETER(ioarg);
  off += td->resource.offset;
  CHECKIO(lseek(td->resource.fd, off, SEEK_SET), off);
  CHECKIO(read(td->resource.fd, buf, siz), siz);
  return 0;
}

static
int td_chunk_write
  (td_t* td, unsigned off, const void* buf, unsigned siz, void* ioarg)
{
  DISCARD_PARAMETER(ioarg);
  off += td->resource.offset;
  CHECKIO(lseek(td->resource.fd, off, SEEK_SET), off);
  CHECKIO(write(td->resource.fd, buf, siz), siz);
  return 0;
}

static
void* td_chunk_realloc
  (td_t* td, void* ptr, unsigned size, void* arg)
{
  DISCARD_PARAMETER(td);
  DISCARD_PARAMETER(arg);
  return realloc(ptr, size);
}

/**
 * \ingroup btree
 *
 * Initializes a btree to use a part of a random-access file.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param flags Bits from the TDCFLG_* values.
 * \param fd Read/write, random-access, open file descriptor.
 * \param off Offset within the file pointed to by fd.
 * \param size Size to use within the file pointed to by fd.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_init_chunk
  (td_t* td, unsigned flags, int fd, unsigned off, unsigned size)
{
  CHECK(td_init(td));
  td->resource.fd = fd;
  td->resource.offset = off;
  td->header.size = size;
  td->lock = td_chunk_lock;
  td->read = td_chunk_read;
  td->write = td_chunk_write;
  if (flags & TDFLG_ALLOCTDT) {
    td->realloc = td_chunk_realloc;
  }
  CHECK(td_init2(td, "tdi", 1, flags));
  return 0;
}

#ifdef __cplusplus
}
#endif
