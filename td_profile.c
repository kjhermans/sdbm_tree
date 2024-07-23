/*
** Copyright 2022 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

#include "td_private.h"

static
int td_prof_lock
  (td_t* td, int how, void* arg)
{
  tdp_t* h = arg;
  struct timeval tv0, tv1, tvd;
  int e;

  gettimeofday(&tv0, 0);
  e = h->lock(td, how, h->lockarg);
  gettimeofday(&tv1, 0);
  timersub(&tv1, &tv0, &tvd);
  dprintf(h->fd, "lock; %lu.%.6lu\n", tvd.tv_sec, tvd.tv_usec);
  return e;
}

static
int td_prof_read
  (td_t* td, unsigned off, void* buf, unsigned siz, void* arg)
{
  tdp_t* h = arg;
  struct timeval tv0, tv1, tvd;
  int e;

  gettimeofday(&tv0, 0);
  e = h->read(td, off, buf, siz, h->ioarg);
  gettimeofday(&tv1, 0);
  timersub(&tv1, &tv0, &tvd);
  dprintf(h->fd, "read; %lu.%.6lu; %u @ %u\n", tvd.tv_sec, tvd.tv_usec, siz, off);
  return e;
}

static
int td_prof_write
  (td_t* td, unsigned off, const void* buf, unsigned siz, void* arg)
{
  tdp_t* h = arg;
  struct timeval tv0, tv1, tvd;
  int e;

  gettimeofday(&tv0, 0);
  e = h->write(td, off, buf, siz, h->ioarg);
  gettimeofday(&tv1, 0);
  timersub(&tv1, &tv0, &tvd);
  dprintf(h->fd, "write; %lu.%.6lu; %u @ %u\n", tvd.tv_sec, tvd.tv_usec, siz, off);
  return e;
}

static
void td_prof_close
  (td_t* td)
{
  tdp_t* h = td->ioarg;
  struct timeval tv0, tv1, tvd;

  gettimeofday(&tv0, 0);
  h->close(td);
  gettimeofday(&tv1, 0);
  timersub(&tv1, &tv0, &tvd);
  dprintf(h->fd, "close; %lu.%.6lu\n", tvd.tv_sec, tvd.tv_usec);
  close(h->fd);
}

static
int td_prof_compare
  (td_t* td, const tdt_t* key1, const tdt_t* key2, int exact, void* arg)
{
  tdp_t* h = arg;
  struct timeval tv0, tv1, tvd;
  int e;

  gettimeofday(&tv0, 0);
  e = h->compare(td, key1, key2, exact, h->cmparg);
  gettimeofday(&tv1, 0);
  timersub(&tv1, &tv0, &tvd);
  dprintf(h->fd, "compare; %lu.%.6lu; k1 %u k2 %u\n", tvd.tv_sec, tvd.tv_usec, key1->size, key2->size);
  return e;
}

static
unsigned td_prof_extend
  (td_t* td, unsigned delta, void* arg)
{
  tdp_t* h = arg;
  struct timeval tv0, tv1, tvd;
  unsigned e;

  gettimeofday(&tv0, 0);
  e = h->extend(td, delta, h->extendarg);
  gettimeofday(&tv1, 0);
  timersub(&tv1, &tv0, &tvd);
  dprintf(h->fd, "extend; %lu.%.6lu; d %u\n", tvd.tv_sec, tvd.tv_usec, delta);
  return e;
}

static
void* td_prof_realloc
  (td_t* td, void* mem, unsigned siz, void* arg)
{
  tdp_t* h = arg;
  struct timeval tv0, tv1, tvd;
  void* ptr;

  gettimeofday(&tv0, 0);
  ptr = h->realloc(td, mem, siz, h->reallocarg);
  gettimeofday(&tv1, 0);
  timersub(&tv1, &tv0, &tvd);
  dprintf(h->fd, "realloc; %lu.%.6lu; size %u\n", tvd.tv_sec, tvd.tv_usec, siz);
  return ptr;
}

#include <fcntl.h>

/**
 * \ingroup btree
 *
 * Allows you to profile the use of btree.
 * Writes a profiling log to the path given by the caller.
 * Works by replacing (and re-using) the callbacks.
 * The file is written as soon as td->close() is called.
 */
int td_profile_start
  (td_t* td, tdp_t* tdp, char* path)
{
  tdp->lock       = td->lock;
  tdp->lockarg    = td->lockarg;
  tdp->read       = td->read;
  tdp->write      = td->write;
  tdp->close      = td->close;
  tdp->ioarg      = td->ioarg;
  tdp->compare    = td->compare;
  tdp->cmparg     = td->cmparg;
  tdp->extend     = td->extend;
  tdp->extendarg  = td->extendarg;
  tdp->realloc    = td->realloc;
  tdp->reallocarg = td->reallocarg;
  tdp->fd         = open(path, O_WRONLY, 0644);
  if (tdp->fd == -1) {
    return TDERR_IO;
  }

  td->lock       = td_prof_lock;
  td->lockarg    = tdp;
  td->read       = td_prof_read;
  td->write      = td_prof_write;
  td->close      = td_prof_close;
  td->ioarg      = tdp;
  td->compare    = td_prof_compare;
  td->cmparg     = tdp;
  td->extend     = td_prof_extend;
  td->extendarg  = tdp;
  td->realloc    = td_prof_realloc;
  td->reallocarg = tdp;

  return 0;
}

void td_profile_stop
  (td_t* td, tdp_t* tdp)
{
  close(tdp->fd);
  td->lock       = tdp->lock;
  td->lockarg    = tdp->lockarg;
  td->read       = tdp->read;
  td->write      = tdp->write;
  td->close      = tdp->close;
  td->ioarg      = tdp->ioarg;
  td->compare    = tdp->compare;
  td->cmparg     = tdp->cmparg;
  td->extend     = tdp->extend;
  td->extendarg  = tdp->extendarg;
  td->realloc    = tdp->realloc;
  td->reallocarg = tdp->reallocarg;
}

#ifdef __cplusplus
}
#endif
