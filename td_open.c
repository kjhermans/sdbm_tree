/*
** Copyright 2015 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

#include "td_private.h"

static
void td_open_close
  (td_t* td)
{
  close(td->resource.fd);
}

/** 
 * \ingroup btree
 *
 * Opens a btree as a file resource, according to POSIX open(2).
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param path Path on the filesystem to open.
 * \param tdflags Bits from the TDFLG_* values.
 * \param openflags Corresponds to the open(2) call flags parameter.
 * \param mode Corresponds to the open(2) call mode parameter.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */ 
int td_open
  (td_t* td, char* path, unsigned tdflags, unsigned openflags, unsigned mode)
{
  int r, fd = open(path, openflags, mode);
  struct stat s;
  if (fstat(fd, &s)) {
    RETURNERR(TDERR_INIT);
  }
  if ((unsigned)(s.st_size) < sizeof(td->header)) {
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    if (tdflags & TDFLG_SPARSE) {
      td->header.size = sizeof(td->header);
    } else {
      td->header.size = sizeof(buf);
    }
    if ((unsigned)write(fd, buf, td->header.size) != td->header.size) {
      RETURNERR(TDERR_IO);
    }
  } else {
    tdflags |= TDFLG_MUSTEXIST;
  }
  tdflags |= TDFLG_EXTEND;
  if ((r = td_init_fd(td, tdflags, fd)) != 0) {
    return r;
  }
  td->close = td_open_close;
  return 0;
}

#ifdef __cplusplus
}
#endif
