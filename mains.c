/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _TD_FD_MAINS_

#include "td_private.h"

//#define unsigned char unsigned char

#include <stdlib.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>

#include <stdio.h>
#include <sys/stat.h>

static
int td_fd_open
  (char* path, int mode, td_t* td)
{
  int fd = open(path, mode, 0644);
  struct stat s;
  if (fstat(fd, &s)) return ~0;
  if (s.st_size == 0) {
    char buf[1024];
    memset(buf, 0, 1024);
    int w = write(fd, buf, 1024); (void)w;
  }
  return td_init_fd(td, TDFLG_EXTEND|TDFLG_ALLOCTDT, fd);
}

#ifdef _TD_FD_PUT_

int main
  (int argc, char* argv[])
{
  td_t td;
  int valuelength = 0;
  char* file = NULL, * key = NULL, * value = NULL;
  char usage[] =
    "Usage: %s [options] file\n"
    "-k <key>\n"
    "-v <value>\n"
    "-f <path>\n";
  int i=1;
  for (; i<argc; i++) {
    char* arg = argv[i];
    if (*arg == '-') {
      ++arg;
      while (*arg) {
        switch (*arg) {
        case 'k':
          if (i < argc - 1) {
            if (key) {
              fprintf(stderr, "Key already set to %s\n", key);
              fprintf(stderr, usage, *argv);
              return ~0;
            } else {
              key = argv[++i];
            }
          } else {
            fprintf(stderr, "-k requires a parameter\n");
            fprintf(stderr, usage, *argv);
            return ~0;
          }
          break;
        case 'v':
          if (i < argc - 1) {
            if (key) {
              value = argv[++i];
              valuelength = strlen(value);
            } else {
              fprintf(stderr, "Key not set\n");
              fprintf(stderr, usage, *argv);
              return ~0;
            }
          } else {
            fprintf(stderr, "-v requires a parameter\n");
            fprintf(stderr, usage, *argv);
            return ~0;
          }
          break;
        }
        ++arg;
      }
    } else if (file) {
      fprintf(stderr, "File already set to %s\n", file);
      fprintf(stderr, usage, *argv);
      return ~0;
    } else {
      file = arg;
    }
  }
  if (file == NULL) {
    fprintf(stderr, "File not set.\n");
    return ~0;
  }
  if (td_fd_open(file, O_RDWR|O_CREAT, &td)) {
    fprintf(stderr, "Couldn't open '%s'\n", file);
    return ~0;
  }
  if (key && value) {
    tdt_t tkey = tdt_string(0, key);
    tdt_t tval = { value, valuelength };
    if (td_put(&td, &tkey, &tval, 0)) {
      fprintf(stderr, "Failed to put '%s'\n", key);
      return ~0;
    }
  } else {
    fprintf(stderr, "Warning: Key and/or value not set.\n");
  }
  return 0;
}

#endif // _TD_FD_PUT_

#ifdef _TD_FD_DUMP_

#include <ctype.h>

int main
  (int argc, char* argv[])
{
  td_t td;
  char* file = NULL;
  char usage[] =
    "Usage: %s [options] file\n";
  int i=1;
  for (; i<argc; i++) {
    char* arg = argv[i];
    if (*arg == '-') {
      ++arg;
      while (*arg) {
        switch (*arg) {
        default:
          fprintf(stderr, usage, *argv);
          return ~0;
        }
        ++arg;
      }
    } else if (file) {
      fprintf(stderr, "File already set to %s\n", file);
      fprintf(stderr, usage, *argv);
      return ~0;
    } else {
      file = arg;
    }
  }
  if (file == NULL) {
    fprintf(stderr, "File not set.\n");
    return ~0;
  }
  if (td_fd_open(file, O_RDWR, &td)) {
    fprintf(stderr, "Couldn't open '%s'\n", file);
    return ~0;
  }
  {
    tdt_t key = { 0, 0 }, val = { 0, 0 };
    tdc_t cursor;
    tdc_init(&td, &cursor);
    while (tdc_nxt(&cursor, &key, &val, TDFLG_ALLOCTDT) == 0) {
      unsigned i;
      fprintf(stdout, "Key \"");
      for (i=0; i < key.size; i++) {
        char c = *((char*)(key.data + i));
        if (isprint(c)) {
          if (c == '\\' || c == '"') {
            fprintf(stdout, "\\");
          }
          fprintf(stdout, "%c", c);
        } else {
          fprintf(stdout, "\\0%.3o", c);
        }
      }
      fprintf(stdout, "\"\nValue \"");
      for (i=0; i < val.size; i++) {
        char c = *((char*)(val.data + i));
        if (isprint(c)) {
          fprintf(stdout, "%c", c);
        } else {
          fprintf(stdout, "\\0%.3o", (unsigned char)c);
        }
      }
      fprintf(stdout, "\"\n----\n");
      free(key.data);
      free(val.data);
      key = (tdt_t){ 0, 0 };
      val = (tdt_t){ 0, 0 };
    }
  }
  return 0;
}

#endif // _TD_FD_DUMP_

#ifdef _TD_FD_DEBUG_

int main
  (int argc, char* argv[])
{
  td_t td;
  char* file = NULL;
  char usage[] =
    "Usage: %s [options] file\n";
  int i=1;
  for (; i<argc; i++) {
    char* arg = argv[i];
    if (*arg == '-') {
      ++arg;
      while (*arg) {
        switch (*arg) {
        default:
          fprintf(stderr, usage, *argv);
          return ~0;
        }
        ++arg;
      }
    } else if (file) {
      fprintf(stderr, "File already set to %s\n", file);
      fprintf(stderr, usage, *argv);
      return ~0;
    } else {
      file = arg;
    }
  }
  if (file == NULL) {
    fprintf(stderr, "File not set.\n");
    return ~0;
  }
  if (td_fd_open(file, O_RDWR, &td)) {
    fprintf(stderr, "Couldn't open '%s'\n", file);
    return ~0;
  }
  return td_debug(&td);
}

#endif // _TD_FD_DEBUG_

#ifdef _TD_FD_AR_

#include "td_private.h"

int main
  (int argc, char* argv[])
{
  td_t td;
  char* archivefile = 0;
  char usage[] =
    "Creates an archive of files\n"
    "Usage: %s [options] <archivefile> <file> [files]\n"
    "Options:\n"
    "-? or -h    Display this text\n"
  ;
  int i=1;

  for (; i < argc; i++) {
    char* arg = argv[i];
    if (*arg == '-') {
      ++arg;
      while (*arg) {
        switch (*arg) {
        case '?':
        case 'h':
          fprintf(stderr, usage, *argv);
          return 0;
        default:
          fprintf(stderr, "Unknown option -%c\n", *arg);
          fprintf(stderr, usage, *argv);
          return ~0;
        }
        ++arg;
      }
    } else {
      archivefile = argv[ i ];
      break;
    }
  }
  if (archivefile == NULL) {
    fprintf(stderr, "File not set.\n");
    return ~0;
  }
  if (td_open(
        &td,
        archivefile,
        TDFLG_EXTEND|TDFLG_SPARSE,
        O_RDWR|O_CREAT|O_TRUNC,
        0644))
  {
    fprintf(stderr, "Couldn't open '%s'\n", archivefile);
    return ~0;
  }
  for (++i; i < argc; i++) {
    if (td_ar_scan(argv[ i ], &td)) {
      fprintf(stderr, "Scanning error.\n");
      return ~0;
    }
  }
  return 0; //td_debug(&td);
}

#endif // !_TO_FD_AR_

#ifdef _TD_FD_ARLIST_

int main
  (int argc, char* argv[])
{
  td_t td;
  char* file = 0;
  int longform = 0;

  char usage[] =
    "Lists an archive of files\n"
    "Usage: %s [options] archivefile\n"
    "Options:\n"
    "-l Long form listing\n"
  ;
  int i=1;

  for (; i<argc; i++) {
    char* arg = argv[i];
    if (*arg == '-') {
      ++arg;
      while (*arg) {
        switch (*arg) {
        case 'l':
          longform = 1;
          break;
        default:
          fprintf(stderr, usage, *argv);
          return ~0;
        }
        ++arg;
      }
    } else if (file) {
      fprintf(stderr, "File already set to %s\n", file);
      fprintf(stderr, usage, *argv);
      return ~0;
    } else {
      file = arg;
    }
  }
  if (file == NULL) {
    fprintf(stderr, "File not set.\n");
    return ~0;
  }

  if (td_open(&td, file, TDFLG_EXTEND, O_RDONLY, 0)) {
    fprintf(stderr, "Couldn't open '%s'\n", file);
    return ~0;
  }
  if (td_ar_list(&td, longform)) {
    fprintf(stderr, "Listing error.\n");
    return ~0;
  }
  return 0;
}

#endif // !_TD_FD_ARLIST_

#ifdef _TD_FD_UNAR_

int main
  (int argc, char* argv[])
{
  td_t td;
  char* file = 0;
  int longform = 0;

  char usage[] =
    "Unpack an archive of files\n"
    "Usage: %s [options] archivefile\n"
    "Options:\n"
    "-l Long form listing\n"
  ;
  int i=1;

  for (; i<argc; i++) {
    char* arg = argv[i];
    if (*arg == '-') {
      ++arg;
      while (*arg) {
        switch (*arg) {
        case 'l':
          longform = 1;
          break;
        default:
          fprintf(stderr, usage, *argv);
          return ~0;
        }
        ++arg;
      }
    } else if (file) {
      fprintf(stderr, "File already set to %s\n", file);
      fprintf(stderr, usage, *argv);
      return ~0;
    } else {
      file = arg;
    }
  }
  if (file == NULL) {
    fprintf(stderr, "File not set.\n");
    return ~0;
  }

  if (td_open(&td, file, TDFLG_EXTEND, O_RDONLY, 0)) {
    fprintf(stderr, "Couldn't open '%s'\n", file);
    return ~0;
  }
  if (td_ar_unpack(&td)) {
    fprintf(stderr, "Unpacking error.\n");
    return ~0;
  }
  return 0;
}

#endif // !_TD_FD_UNAR_

#endif // _TD_FD_MAINS_

#ifdef __cplusplus
}
#endif
