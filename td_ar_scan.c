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

#include "td_private.h"
#include "td_ar.h"

static
int td_ar_create_key
  (
    char* path,
    struct stat* s,
    unsigned char keydata[ TD_AR_KEY_MAX ],
    unsigned* keydatasize
  )
{
  td_ar_stat_t ars;
  
  ars.magic = htonl(TDAR_MAGIC);
  if ((s->st_mode & S_IFMT) == S_IFDIR) {
    ars.type = htons(TDAR_TYP_DIR);
  } else if ((s->st_mode & S_IFMT) == S_IFLNK) {
    ars.type = htons(TDAR_TYP_SYMLINK);
  } else if ((s->st_mode & S_IFMT) == S_IFREG) {
    ars.type = htons(TDAR_TYP_FILE);
  }
  ars.bits = 0;
  if ((s->st_mode & S_IRUSR) == S_IRUSR) ars.bits |= TDAR_IRUSR;
  if ((s->st_mode & S_IWUSR) == S_IWUSR) ars.bits |= TDAR_IWUSR;
  if ((s->st_mode & S_IXUSR) == S_IXUSR) ars.bits |= TDAR_IXUSR;
  if ((s->st_mode & S_IRGRP) == S_IRGRP) ars.bits |= TDAR_IRGRP;
  if ((s->st_mode & S_IWGRP) == S_IWGRP) ars.bits |= TDAR_IWGRP;
  if ((s->st_mode & S_IXGRP) == S_IXGRP) ars.bits |= TDAR_IXGRP;
  if ((s->st_mode & S_IROTH) == S_IROTH) ars.bits |= TDAR_IROTH;
  if ((s->st_mode & S_IWOTH) == S_IWOTH) ars.bits |= TDAR_IWOTH;
  if ((s->st_mode & S_IXOTH) == S_IXOTH) ars.bits |= TDAR_IXOTH;
  ars.bits = htons(ars.bits);
  ars.uid = htonl(s->st_uid);
  ars.gid = htonl(s->st_gid);
  ars.size = htobe64(s->st_size);
  ars.ctime = htobe64(s->st_ctime);
  ars.mtime = htobe64(s->st_mtime);
  ars.atime = htobe64(s->st_atime);
  memcpy(keydata, &ars, sizeof(ars));
  snprintf(
    (char*)(&(keydata[ sizeof(ars) ])),
    TD_AR_KEY_MAX - sizeof(ars),
    "%s",
    path
  );
  *keydatasize = strlen((char*)(&(keydata[ sizeof(ars) ]))) + sizeof(ars);
  return 0;
}

static
int td_ar_scan_file
  (char* path, struct stat* s, td_t* td)
{
  unsigned char keydata[ TD_AR_KEY_MAX ];
  tdt_t key = { keydata, sizeof(keydata) };
  int fd = open(path, O_RDONLY, 0);

  CHECK(td_ar_create_key(path, s, keydata, &(key.size)));
  if (fd >= 0) {
    int e = td_put_stream(td, &key, fd, 0);
    if (e == 0) {
      return 0;
    }
  }
  return ~0;
}

static
int td_ar_scan_link
  (char* path, struct stat* s, td_t* td)
{
  unsigned char keydata[ TD_AR_KEY_MAX ];
  char valuedata[ TD_AR_PATH_MAX ];
  tdt_t key = { keydata, sizeof(keydata) };
  tdt_t value = { valuedata, sizeof(valuedata) };
  ssize_t linksize = readlink(path, valuedata, sizeof(valuedata));
  
  value.size = linksize;
  CHECK(td_ar_create_key(path, s, keydata, &(key.size)));
  int e = td_put(td, &key, &value, 0);
  if (e == 0) {
    return 0;
  }
  return ~0;
}

int td_ar_scan
  (char* path, td_t* td);

static
int td_ar_scan_dir
  (char* path, struct stat* s, td_t* td)
{
  char entrypath[ 1024 ];
  struct dirent* entry;
  DIR* dir = opendir(path);
  (void)s;

  if (dir == NULL) {
    fprintf(stderr, "Directory %s could not be opened.\n", path);
    return ~0;
  }
  fprintf(stderr, "Scanning directory %s\n", path);
  while ((entry = readdir(dir)) != NULL) {
    if (!strcmp(entry->d_name, ".")) {
      continue;
    }
    if (!strcmp(entry->d_name, "..")) {
      continue;
    }
    snprintf(entrypath, sizeof(entrypath), "%s/%s", path, entry->d_name);
    if (td_ar_scan(entrypath, td)) {
      fprintf(stderr,
        "Error scanning path %s in directory %s\n", entrypath, path
      );
      return ~0;
    }
  }
  closedir(dir);
  return 0;
}

int td_ar_scan
  (char* path, td_t* td)
{
  struct stat s;

  if (lstat(path, &s)) {
    return ~0;
  }
  if ((s.st_mode & S_IFMT) == S_IFDIR) {
    fprintf(stderr, "Scanning dir %s\n", path);
    if (td_ar_scan_dir(path, &s, td)) {
      fprintf(stderr, "Error processing dir %s\n", path);
      return ~0;
    }
  } else if ((s.st_mode & S_IFLNK) == S_IFLNK) {
    fprintf(stderr, "Scanning symlink %s\n", path);
    if (td_ar_scan_link(path, &s, td)) {
      fprintf(stderr, "Error processing link %s\n", path);
      return ~0;
    }
  } else if ((s.st_mode & S_IFMT) == S_IFREG) {
    fprintf(stderr, "Scanning file %s\n", path);
    if (td_ar_scan_file(path, &s, td)) {
      fprintf(stderr, "Error processing file %s\n", path);
      return ~0;
    }
  } else {
    fprintf(stderr, "WARNING: Unhandled file %s\n", path);
  }
  return 0;
}
