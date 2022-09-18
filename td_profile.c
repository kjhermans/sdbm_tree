/*
** Copyright 2022 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

struct td_profhelp
{
  int             (*lock)(td_t*,int,void*);
  void*             lockarg;
  int             (*read)(td_t*, unsigned, void*, unsigned, void*);
  int             (*write)(td_t*, unsigned, const void*, unsigned, void*);
  void            (*close)(td_t*);
  void*             ioarg;
  int             (*compare)(td_t*,const tdt_t*,const tdt_t*,int,void*);
  void*             cmparg;
  unsigned        (*extend)(td_t*,unsigned,void*);
  void*             extendarg;
  void*           (*realloc)(td_t*,void*,unsigned,void*);
  void*             reallocarg;
  td_t              profile;
  unsigned          counter;
};

static
int td_prof_lock
  (td_t* td, int how, void* arg)
{
  struct td_profhelp* h = arg;
  int e = h->lock(td, how, h->lockarg);
  if (e) {
    //.. admin
    return e;
  }
  //.. admin
  return 0;
}

static
int td_prof_read
  (td_t* td, unsigned off, void* buf, unsigned siz, void* arg)
{
  struct td_profhelp* h = arg;
  int e = h->read(td, off, buf, siz, h->ioarg);
  if (e) {
    //.. admin
    return e;
  }
  //.. admin
  return 0;
}

static
int td_prof_write
  (td_t* td, unsigned off, const void* buf, unsigned siz, void* arg)
{
  struct td_profhelp* h = arg;
  int e = h->write(td, off, buf, siz, h->ioarg);
  if (e) {
    //.. admin
    return e;
  }
  //.. admin
  return 0;
}

static
void td_prof_close
  (td_t* td)
{
  struct td_profhelp* h = td->ioarg;

  h->close(td);
  //.. write to h->path
}

static
int td_prof_compare
  (td_t* td, const tdt_t* key1, const tdt_t* key2, int exact, void* arg)
{
  struct td_profhelp* h = arg;
  int e = h->compare(td, key1, key2, exact, h->cmparg);

  if (e) {
    //.. admin
    return e;
  }
  //.. admin
  return 0;
}

static
unsigned td_prof_extend
  (td_t* td, unsigned delta, void* arg)
{
  struct td_profhelp* h = arg;
  unsigned e = h->extend(td, delta, h->extendarg);

  //.. admin
  return e;
}

static
void* td_prof_realloc
  (td_t* td, void* mem, unsigned siz, void* arg)
{
  struct td_profhelp* h = arg;
  void* ptr = h->realloc(td, mem, siz, h->reallocarg);

  //.. admin
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
int td_profile
  (td_t* td, char* path)
{
  struct td_profhelp h = {
    .lock       = td->lock,
    .lockarg    = td->lockarg,
    .read       = td->read,
    .write      = td->write,
    .close      = td->close,
    .ioarg      = td->ioarg,
    .compare    = td->compare,
    .cmparg     = td->cmparg,
    .extend     = td->extend,
    .extendarg  = td->extendarg,
    .realloc    = td->realloc,
    .reallocarg = td->reallocarg,
    .counter    = 0
  };
  td->lock       = td_prof_lock;
  td->lockarg    = &h;
  td->read       = td_prof_read;
  td->write      = td_prof_write;
  td->close      = td_prof_close;
  td->ioarg      = &h;
  td->compare    = td_prof_compare;
  td->cmparg     = &h;
  td->extend     = td_prof_extend;
  td->extendarg  = &h;
  td->realloc    = td_prof_realloc;
  td->reallocarg = &h;
  if (td_open(&(h.profile), path, 0, O_RDWR|O_CREAT|O_TRUNC, 0640)) {
    return ~0;
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
