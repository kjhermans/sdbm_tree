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
int td_fd_lock
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
int td_fd_read
  (td_t* td, unsigned off, void* data, unsigned size, void* ioarg)
{
  DISCARD_PARAMETER(ioarg);
  CHECKIO(lseek(td->resource.fd, off, SEEK_SET), off);
  while (size) {
    int r, l = 1024;
    if (size < 1024) {
      l = size;
    }
    if ((r = read(td->resource.fd, data, l)) <= 0) {
      RETURNERR(TDERR_IO);
    }
    size -= r;
    data += r;
  }
  return 0;
}

static
int td_fd_write
  (td_t* td, unsigned off, const void* data, unsigned size, void* ioarg)
{
  DISCARD_PARAMETER(ioarg);
  CHECKIO(lseek(td->resource.fd, off, SEEK_SET), off);
  while (size) {
    int r, l = 1024;
    if (size < 1024) {
      l = size;
    }
    if ((r = write(td->resource.fd, data, l)) <= 0) {
      RETURNERR(TDERR_IO);
    }
    size -= r;
    data += r;
  }
  return 0;
}

static
unsigned td_fd_extend
  (td_t* td, unsigned d, void* arg)
{
  DISCARD_PARAMETER(arg);
  unsigned cursize;
  unsigned r = 0;
  char mem[1024];
  memset(mem, 0, 1024);
  if ((cursize = lseek(td->resource.fd, 0, SEEK_END)) == (unsigned)-1) {
    return 0;
  }
  td->header.size = cursize;
  /* align d with kilobytes */
  if (!(td->header.flags & TDFLG_SPARSE) && ((cursize + d) % 1024)) {
    d += (1024 - ((cursize + d) % 1024));
  }
  while (d > 1024) {
    int w = write(td->resource.fd, mem, 1024);
    if (w <= 0) break;
    r += w;
    d -= 1024;
  }
  {
    int w = write(td->resource.fd, mem, d);
    r += w;
  }
  return r;
}

static
void* td_fd_realloc
  (td_t* td, void* ptr, unsigned size, void* arg)
{
  DISCARD_PARAMETER(td);
  DISCARD_PARAMETER(arg);
  return realloc(ptr, size);
}

/**
 * \ingroup btree
 *
 * Initializes a btree to use a random-access file-descriptor as its resource.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param flags Bits from the TDFLG_* values.
 * \param fd A read/write random-access open filedescriptor.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_init_fd
  (td_t* td, unsigned flags, int fd)
{
  CHECK(td_init(td));
  if ((td->header.size = lseek(fd, 0, SEEK_END)) == (unsigned)-1) {
    RETURNERR(TDERR_INIT);
  }
  td->resource.fd = fd;
  td->lock = td_fd_lock;
  td->read = td_fd_read;
  td->write = td_fd_write;
  if (flags & TDFLG_EXTEND) {
    td->extend = td_fd_extend;
  }
  if (flags & TDFLG_ALLOCTDT) {
    td->realloc = td_fd_realloc;
  }
  CHECK(td_init2(td, "tdi", TD_DISK_ALIGN, flags));
  return 0;
}

#ifdef __cplusplus
}
#endif
