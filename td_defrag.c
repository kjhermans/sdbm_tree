/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
int td_sort_empties_from
  (td_t* td, unsigned level, unsigned orig, unsigned elt1, unsigned elt2)
{
  unsigned list[ level ], length = 1;
  unsigned ptr = elt2, last = 0;
  list[0] = elt1;
  while ((length < level) && (ptr != 0)) {
    list[ length++ ] = ptr;
    CHECK(td_read_uint(td, ptr, &last));
    ptr = last;
  }
  if (length < 2) {
    return 0;
  }
  td_qsort(list, length);
  if (orig == 0) {
    td->header.off_e = list[ 0 ];
  } else {
    CHECK(td_write_uint(td, orig, list[ 0 ]));
  }
  orig = list[ 0 ];
  unsigned i;
  for (i=1; i < length; i++) {
    CHECK(td_write_uint(td, list[ i-1 ], list[ i ]));
  }
  CHECK(td_write_uint(td, list[ i-1 ], last));
  return 0;
}

static
int td_sort_empties
  (td_t* td, unsigned level)
{
  if (level < 2) {
    return 0;
  }
  if (td->header.off_e == 0) {
    return 0;
  }
  unsigned ptr = td->header.off_e, orig = 0;
  unsigned counter = 0;
  while ((ptr != 0) && (counter++ < level)) {
    unsigned next;
    CHECK(td_read_uint(td, ptr, &next));
    if (next && next < ptr) {
      CHECK(td_sort_empties_from(td, level, orig, ptr, next));
      return 0;
    }
    orig = ptr;
    ptr = next;
  }
  return 0; /* sorted already */
}

static
int td_merge_empties
  (td_t* td, unsigned level)
{
  unsigned ptr1 = td->header.off_e, ptr2;
  struct chunkhead head1, head2;
  if (ptr1) {
    CHECK(td_read_chunkhead(td, ptr1, &head1));
    ptr2 = head1.next;
    while (ptr2 && level) {
      CHECK(td_read_chunkhead(td, ptr2, &head2));
      if (ptr1 + head1.size == ptr2) {
        head1.size += head2.size;
        head1.next = head2.next;
        CHECK(td_write_chunkhead(td, ptr1, &head1));
        --level;
        --(td->header.nempties);
      } else {
        ptr1 = ptr2;
        head1 = head2;
      }
      ptr2 = head2.next;
    }
  }
  return 0;
}

/**
 * \ingroup btree_private
 * Defragments our space, both alignment of empty chunks and fragmented
 * values. This function is called
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_defrag
  (td_t* td)
{
  if (td->header.nempties == 0) {
    return 0;
  }
  unsigned level = (td->header.nempties * td->header.nempties);
  if (level > 2) {
    if (level > MAX_DEFRAGLEVEL) {
      level = MAX_DEFRAGLEVEL;
    }
    CHECK(td_sort_empties(td, level));
    CHECK(td_merge_empties(td, level));
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
