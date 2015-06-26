/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifndef _TD_PRIVATE_H_
#define _TD_PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _TD_PRIVATE_
#include "td.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DISCARD_PARAMETER(p) (void)p

#define TD_OFF_MAX (unsigned)(~0)

int td_init2(td_t* td, const char* ident, unsigned align, unsigned int flags);

#ifdef _DEBUG
#define WARN \
  fprintf(stderr, "Error %d, file=%s, line=%u\n", (__r), __FILE__, __LINE__)
#endif

#ifndef CHECK
#ifdef _DEBUG
#define CHECK(a) { int __r; if ((__r = (a))) { WARN; return __r; } }
#else
#define CHECK(a) { int __r; if ((__r = (a))) { return __r; } }
#endif
#endif

#ifndef CHECK2
#define CHECK2(a,b) { int __r; if ((__r = (a))) { (b); return __r; } }
#endif

#ifndef RETURNERR
#ifdef _DEBUG
#define RETURNERR(e) { \
  fprintf(stderr, "Error code %d in %s at %d\n", e, __FILE__, __LINE__); \
  return(e); \
}
#else
#define RETURNERR(e) { \
  if ((e) < 0) { \
    fprintf(stderr, "Error code %d in %s at %d\n", e, __FILE__, __LINE__); \
  } \
  return(e); \
}
#endif
#endif

typedef unsigned char uchar;

#ifndef TD_DISK_ALIGN
//#define TD_DISK_ALIGN 512
#define TD_DISK_ALIGN 1
#endif

#include "td_functions.h"

static inline unsigned int td_align(unsigned int a, unsigned int n) {
  if ((a > 1) && (n % a)) {
    return (((n / a) + 1) * a);
  } else {
    return n;
  }
}

#ifndef CHECKIO
#define CHECKIO(fnc,usiz) { \
  off_t __n = (fnc); \
  if (__n < 0) { \
    return TDERR_IO; \
  } \
  if ((unsigned)__n != (usiz)) { \
    return TDERR_IO; \
  } \
}
#endif

#define MAX_DEFRAGLEVEL                 512

#ifdef __cplusplus
}
#endif

#endif
