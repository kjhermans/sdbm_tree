/**
 * This file is part of Oroszlan, a parsing and scripting environment

Copyright (c) 2023, Kees-Jan Hermans <kees.jan.hermans@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the organization nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Kees-Jan Hermans BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *
 * \file
 * \brief
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

#include <naigama/util/td.h>

void tdt_printf
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
  vsnprintf((char*)(out->data) + out->size, size, fmt, ap);
  va_end(ap);
  out->size += (size - 1);
}

void tdt_append
  (tdt_t* out, void* mem, unsigned size)
{
  out->data = realloc(out->data, out->size + size + 1);
  memcpy(out->data + out->size, mem, size);
  out->size += size;
  ((char*)(out->data))[ out->size ] = 0;
}

void tdt_appendchr
  (tdt_t* out, unsigned char c)
{
  td_append(out, &c, 1);
}

void tdt_appendstr
  (tdt_t* out, char* str)
{
  td_append(out, str, strlen(str));
}

void tdt_prepend
  (tdt_t* out, void* mem, unsigned size)
{
  out->data = realloc(out->data, out->size + size + 1);
  memmove(out->data + size, out->data, out->size + 1);
  memcpy(out->data, mem, size);
  out->size += size;
}

void tdt_insert
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

void tdt_reserve
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

void tdt_copy
  (tdt_t* out, tdt_t* in)
{
  out->data = malloc(in->size + 1);
  memcpy(out->data, in->data, in->size);
  out->size = in->size;
  *((uint8_t*)(out->data + out->size)) = 0;
}

void tdt_shift
  (tdt_t* out, unsigned offset, unsigned whereto, int size)
{
  if (offset >= out->size) { return; } // don't know how to do that
  if (size == -1 || offset + size > out->size) {
    size = out->size - offset;
  }
  if (offset < whereto) {
    unsigned diff = whereto - offset;
    out->data = realloc(out->data, out->size + diff + 1);
    memmove(out->data + whereto, out->data + offset, size + 1);
    memset(out->data + offset, 0, diff);
    out->size += (offset - whereto);
  } else if (offset > whereto) {
    memmove(out->data + whereto, out->data + offset, size + 1);
    out->size -= (offset - whereto);
  }
}
