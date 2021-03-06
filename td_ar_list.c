/*
** Copyright 2018 K.J. Hermans (kees.jan.hermans@gmail.com)
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

int td_ar_list
  (td_t* td, int longform)
{
  unsigned char keydata[ TD_AR_KEY_MAX ];
  tdc_t cursor;
  tdt_t key = { keydata, sizeof(keydata) };
  tdc_init(td, &cursor);
  while (tdc_get(&cursor, &key, 0, 0) == 0) {
    td_ar_stat_t s;
    char* filename;
    unsigned filenamesize;
    char linkstring[ 1024 ];
    if (key.size < sizeof(s)) {
      fprintf(stderr, "Key size too small.\n");
      return ~0;
    }
    memcpy(&s, keydata, sizeof(s));
    if (s.magic != htonl(0x74647200)) {
      fprintf(stderr, "File is not a td archive.\n");
      return ~0;
    }
    s.type = htons(s.type);
    s.bits = htons(s.bits);
    s.uid = htonl(s.uid);
    s.gid = htonl(s.gid);
    s.size = htobe64(s.size);
    s.ctime = htobe64(s.ctime);
    s.mtime = htobe64(s.mtime);
    s.atime = htobe64(s.atime);
    filename = (char*)(&(keydata[ sizeof(s) ]));
    filenamesize = key.size - sizeof(s);
    if (s.type == TDAR_TYP_SYMLINK) {
      tdt_t val = { &(linkstring[ 4 ]), sizeof(linkstring)-5 };
      tdc_get(&cursor, 0, &val, 0);
      linkstring[ 0 ] = ' ';
      linkstring[ 1 ] = '-';
      linkstring[ 2 ] = '>';
      linkstring[ 3 ] = ' ';
      linkstring[ val.size + 4 ] = 0;
    } else {
      linkstring[ 0 ] = 0;
    }
    if (longform) {
      fprintf(stdout,
        "%s %5u %5u %12" PRIu64 " %s %-.*s%s\n"
        , td_ar_modestring(s.type, s.bits)
        , s.uid
        , s.gid
        , s.size
        , td_ar_datestring(s.mtime)
        , filenamesize, filename
        , linkstring
      );
    } else {
      fprintf(stdout, "%-.*s\n", filenamesize, filename);
    }
    key.size = sizeof(keydata);
    tdc_nxt(&cursor, 0, 0, 0);
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
