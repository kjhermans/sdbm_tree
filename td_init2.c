/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
int td_init_create
  (td_t* td, const char* ident, unsigned align, unsigned flags)
{
  unsigned off_topnode = 0;
  unsigned off_echunk = td_align(align, sizeof(struct td_header));
  struct chunkhead emptychunk = { 0, td->header.size - off_echunk, 0, 0 };
  td->header = (struct td_header){
    { 0, 0, 0, 0 },             /* magic */
    flags,                      /* flags */
    td->header.size,            /* our space */
    align,                      /* alignment in bytes */
    off_topnode,                /* top node starts just beyond us */
    1,                          /* our initial level is one */
    off_echunk,                 /* empty chunk starts just beyond header */
    0,                          /* scn */
    0,                          /* 0 entries */
    1,                          /* 1 empty chunk (the remainder) */
    0                           /* number of value chunks */
  };
  td->header.magic[0] = ident[0];
  td->header.magic[1] = ident[1];
  td->header.magic[2] = ident[2];
  CHECK(td_write_header(td));
  CHECK(td_write_chunkhead(td, off_echunk, &emptychunk));
  return 0;
}

static
int td_init_create_sparse
  (td_t* td, const char* ident, unsigned align, unsigned flags)
{
  unsigned off_topnode = 0;
  td->header = (struct td_header){
    { 0, 0, 0, 0 },             /* magic */
    flags,                      /* flags */
    td->header.size,            /* our space */
    align,                      /* alignment in bytes */
    off_topnode,                /* top node starts just beyond us */
    1,                          /* our initial level is one */
    0,                          /* empty chunk starts just beyond header */
    0,                          /* scn */
    0,                          /* 0 entries */
    0,                          /* 1 empty chunk (the remainder) */
    0                           /* number of value chunks */
  };
  td->header.magic[0] = ident[0];
  td->header.magic[1] = ident[1];
  td->header.magic[2] = ident[2];
  CHECK(td_write_header(td));
  return 0;
}

/**
 * \ingroup bree_private
 *
 * Back-end function of all td_init_* functions.
 * 
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param ident Magic three-byte string dictating type of file ("tdi")
 * \param align Make the btree always a multiple of this amount of bytes.
 * \param flags Bits from the TDFLG_* values.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_init2
  (td_t* td, const char* ident, unsigned align, unsigned flags)
{
  struct td_header header;
  if (td->read(td, 0, (char*)(&header), sizeof(struct td_header), td->ioarg)
      || memcmp(header.magic, ident, 4))
  {
    if (flags & TDFLG_MUSTEXIST) {
      RETURNERR(TDERR_INIT);
    } else {
      if (flags & TDFLG_SPARSE) {
        CHECK(td_init_create_sparse(td, ident, 1, flags));
      } else {
        CHECK(td_init_create(td, ident, align, flags));
      }
    }
  } else {
    td->header = header;
  }
  td->compare = td_compare;
  TDLOG("%s flags=%u size=%u align=%u top=%u level=%u off_e=%u scn=%u\n",
    __FILE__,
    td->header.flags,
    td->header.size,
    td->header.align,
    td->header.top,
    td->header.level,
    td->header.off_e,
    td->header.scn
  );
  return 0;
}

#ifdef __cplusplus
}
#endif
