/*
** Copyright 2018 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <arpa/inet.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <endian.h>
#include <inttypes.h>

#include <time.h>

#include "td_private.h"
#include "td_ar.h"

/*
static
char md[ 32 ];

static
char* td_ar_modestring
  (uint16_t type, uint16_t flags)
{
  memset(md, '-', 10);
  switch (type) {
  case TDAR_TYP_FILE: md[ 0 ] = '-'; break;
  case TDAR_TYP_DIR: md[ 0 ] = 'd'; break;
  case TDAR_TYP_SYMLINK: md[ 0 ] = 'l'; break;
  }
  if ((flags & TDAR_IRUSR) == TDAR_IRUSR) {
    md[ 1 ] = 'r';
  }
  if ((flags & TDAR_IWUSR) == TDAR_IWUSR) {
    md[ 2 ] = 'w';
  }
  if ((flags & TDAR_IXUSR) == TDAR_IXUSR) {
    md[ 3 ] = 'x';
  }
  if ((flags & TDAR_IRGRP) == TDAR_IRGRP) {
    md[ 4 ] = 'r';
  }
  if ((flags & TDAR_IWGRP) == TDAR_IWGRP) {
    md[ 5 ] = 'w';
  }
  if ((flags & TDAR_IXGRP) == TDAR_IXGRP) {
    md[ 6 ] = 'x';
  }
  if ((flags & TDAR_IROTH) == TDAR_IROTH) {
    md[ 7 ] = 'r';
  }
  if ((flags & TDAR_IWOTH) == TDAR_IWOTH) {
    md[ 8 ] = 'w';
  }
  if ((flags & TDAR_IXOTH) == TDAR_IXOTH) {
    md[ 9 ] = 'x';
  }
  md[ 10 ] = 0;
  return md;
}

static
char ds[ 64 ];

static
char* mon[] = {
  "jan", "feb", "mar", "apr", "may", "jun",
  "jul", "aug", "sep", "oct", "nov", "dec"
};

static
char* td_ar_datestring
  (uint64_t date)
{
  time_t now = time(0);
  time_t file = (time_t)date;
  struct tm* tm = localtime(&file);

  if (file < now - (365 * 86400)) {
    snprintf(ds, sizeof(ds),
      "%s %2u %5d", mon[ tm->tm_mon ], tm->tm_mday, tm->tm_year + 1900
    );
  } else {
    snprintf(ds, sizeof(ds),
      "%s %2u %.2u:%.2u"
      , mon[ tm->tm_mon ], tm->tm_mday, tm->tm_hour, tm->tm_min
    );
  }
  return ds;
}
*/

int td_ar_unpack
  (td_t* td)
{
  unsigned char keydata[ TD_AR_KEY_MAX ];
  tdt_t key = { keydata, sizeof(keydata) };
  tdc_t cursor;

  tdc_init(td, &cursor);
  while (tdc_get(&cursor, &key, 0, 0) == 0) {
    char path[ 2048 ];
    td_ar_stat_t s;
    char* filename;
    unsigned filenamesize;

    if (key.size < sizeof(s)) {
      fprintf(stderr, "Key size too small.\n");
      return ~0;
    }
    memcpy(&s, keydata, sizeof(s));
    if (s.magic != htonl(0x74647200)) {
      fprintf(stderr, "File is not a td archive.\n");
      return ~0;
    }
    s.bits = htons(s.bits);
    s.type = htons(s.type);
    s.uid = htonl(s.uid);
    s.gid = htonl(s.gid);
    s.size = htobe64(s.size);
    s.ctime = htobe64(s.ctime);
    s.mtime = htobe64(s.mtime);
    s.atime = htobe64(s.atime);
    filename = (char*)(&(keydata[ sizeof(s) ]));
    filenamesize = key.size - sizeof(s);
    snprintf(path, sizeof(path), "%-.*s", filenamesize, filename);
    fprintf(stdout, "%s\n", path);
    if (s.type == TDAR_TYP_FILE) {
      int fd;
      if ((fd = open(path, O_WRONLY|O_CREAT|O_TRUNC, s.bits)) < 0) {
        fprintf(stderr, "Couldn't open file %s\n", path);
        return ~0;
      }
      if (tdc_get_stream(&cursor, 0, fd, 0)) {
        close(fd);
        fprintf(stderr, "Error writing file %s\n", path);
        return ~0;
      }
      close(fd);
    } else if (s.type == TDAR_TYP_SYMLINK) {
      char link[ 1024 ];
      tdt_t value = { link, sizeof(link)-1 };
      if (tdc_get(&cursor, 0, &value, 0) == 0) {
        link[ value.size ] = 0;
        unlink(path);
        if (symlink(link, path)) {
          fprintf(stderr, "Error creating symlink %s -> %s\n", path, link);
          return ~0;
        }
      }
    } else {
fprintf(stderr, "WTH %u\n", s.type);
    }
    if (tdc_nxt(&cursor, 0, 0, 0)) {
      break;
    }
    key.size = sizeof(keydata);
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
