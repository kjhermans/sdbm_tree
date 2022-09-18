/*
** Copyright 2022 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdarg.h>

/**
 * \ingroup btree
 * Various functions to aid with creating tdt_t's.
 */

void td_printf
  (tdt_t* out, char* fmt, ...)
{
  va_list ap;
  int size;

  va_start(ap, fmt);
  size = vsnprintf(0, 0, fmt, ap);
  va_end(ap);
  if (size < 0) {
    return;
  }
  ++size;
  va_start(ap, fmt);
  out->data = realloc(out->data, out->size + size);
  vsnprintf(out->data + out->size, size, fmt, ap);
  va_end(ap);
  out->size += (size - 1);
}

void td_append
  (tdt_t* out, void* mem, unsigned size)
{
  out->data = realloc(out->data, out->size + size + 1);
  memcpy(out->data + out->size, mem, size);
  out->size += size;
  ((char*)(out->data))[ out->size ] = 0;
}

void td_appendchr
  (tdt_t* out, unsigned char c)
{
  td_append(out, &c, 1);
}

void td_appendstr
  (tdt_t* out, char* str)
{
  td_append(out, str, strlen(str));
}

void td_prepend
  (tdt_t* out, void* mem, unsigned size)
{
  out->data = realloc(out->data, out->size + size + 1);
  memmove(out->data + size, out->data, out->size + 1);
  memcpy(out->data, mem, size);
  out->size += size;
}

void td_insert
  (tdt_t* out, int offset, void* mem, unsigned size)
{
  if (offset == -1) {
    td_append(out, mem, size);
  } else {
    unsigned remainder;
    if (out->size) { offset %= out->size; }
    out->data = realloc(out->data, out->size + size + 1);
    remainder = out->size - offset;
    if (remainder) {
      memmove(out->data + offset + size, out->data + offset, remainder + 1);
    }
    memcpy(out->data + offset, mem, size);
    out->size += size;
  }
}

void td_reserve
  (tdt_t* out, int offset, unsigned size)
{
  if (offset == -1) {
    out->data = realloc(out->data, out->size + size + 1);
    memset(out->data + out->size, 0, size + 1);
    out->size += size;
  } else {
    unsigned remainder;
    if (out->size) { offset %= out->size; }
    out->data = realloc(out->data, out->size + size + 1);
    remainder = out->size - offset;
    if (remainder) {
      memmove(out->data + offset + size, out->data + offset, remainder + 1);
    }
    memset(out->data + offset, 0, size);
    out->size += size;
  }
}

void td_copy
  (tdt_t* out, tdt_t* in)
{
  out->data = malloc(in->size + 1);
  memcpy(out->data, in->data, in->size);
  out->size = in->size;
  *((uint8_t*)(out->data + out->size)) = 0;
}

#ifdef __cplusplus
}
#endif
