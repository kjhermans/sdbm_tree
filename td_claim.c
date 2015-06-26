/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

#define NFITRECORDS 8

typedef struct fitrec {
  unsigned ptr;
  unsigned nxt;
  unsigned siz;
  unsigned lst;
  unsigned dif;
} fitrec_t;

static inline
int td_claim_fit
  (unsigned diff, unsigned nicest)
{
  if (diff > nicest) {
    return 1;
  }
  return 0;
}

static
int td_claim_(
  td_t* td,
  int contiguous,
  unsigned* off,
  unsigned* size
) {

  if (td->header.off_e == 0) {
    return TDERR_SPACE;
  }

  *size = td_align(td->header.align, *size);

  /* Building the fitness table (or finding a perfect fit */

  unsigned ptr = td->header.off_e, last = ptr;
  fitrec_t fitrec[ NFITRECORDS ];
  unsigned noptions = 0;
  while (ptr) {
    struct chunkhead chunkhead;
    CHECK(td_read_chunkhead(td, ptr, &chunkhead));

    /* bingo, perfect fit ! */
    if (*size == chunkhead.size) {
      if (last == ptr) {
        td->header.off_e = chunkhead.next;
      } else {
        CHECK(td_write_uint(td, last, chunkhead.next));
      }
      *off = ptr;
      --(td->header.nempties);
      return 0;

    /* otherwise, a chunk may be conveniently longer */
    } else if (*size <= chunkhead.size - sizeof(struct chunkhead)) {
      fitrec[ noptions++ ] = (fitrec_t){
        ptr,
        chunkhead.next,
        chunkhead.size,
        last,
        (chunkhead.size - sizeof(struct chunkhead)) - *size
      };

    /* otherwise, a chunk may be conveniently shorter */
    } else if (*size > chunkhead.size && !contiguous) {
      fitrec[ noptions++ ] = (fitrec_t){
        ptr,
        chunkhead.next,
        chunkhead.size,
        last,
        *size - chunkhead.size
      };
    }
    if (noptions == NFITRECORDS) {
      break;
    }
    last = ptr;
    ptr = chunkhead.next;
  }

  /* Judging and using the fitness table */

  if (!noptions) {
    return TDERR_SPACE;
  } else {
    unsigned i = 0, nicest = 0, choice = 0;
    for (; i < noptions; i++) {
      if (td_claim_fit(fitrec[i].dif, nicest)) {
        nicest = fitrec[i].dif;
        choice = i;
      }
    }

    /* split the empty chunk in two */
    if (fitrec[choice].siz > *size) {
      unsigned emptyoff = fitrec[choice].ptr + *size;
      struct chunkhead emptychunk = {
        fitrec[choice].nxt,
        fitrec[choice].siz - *size,
        0,
        0
      };
      CHECK(td_write_chunkhead(td, emptyoff, &emptychunk));
      if (fitrec[choice].lst == fitrec[choice].ptr) {
        td->header.off_e = emptyoff;
      } else {
        CHECK(td_write_uint(td, fitrec[choice].lst, emptyoff));
      }
      *off = fitrec[choice].ptr;
      return 0;

    /* use up this chunk, but change *size */
    } else {
      if (fitrec[choice].lst == fitrec[choice].ptr) {
        td->header.off_e = fitrec[choice].nxt;
      } else {
        CHECK(td_write_uint(td, fitrec[choice].lst, fitrec[choice].nxt));
      }
      *off = fitrec[choice].ptr;
      *size = fitrec[choice].siz;
      --(td->header.nempties);
      return 0;
    }
  }
}

/**
 * \ingroup btree_private
 * Claims an amount of resources.  While iterating through the linked list
 * of empty chunks, fills up a little table denoting the fittingness of
 * this chunk to be used.  Perfect fits are used immediately, while
 * not-so-perfect fits fill up the fitness table.  In the end, when either
 * the table is full or the end of the list has been reached, it makes a
 * choice for the nicest record of the table, and uses the data in this
 * record to make changes.
 *
 * \param td Non-NULL pointer to the initialized td_t structure.
 * \param contiguous Whether or not the value of *size must be met whole.
 * \param off Returns the offset on success
 * \param size Filled with requested size on calling, filled with granted
 * size on return.  NB. The requested size must be the full amount of space
 * requested, not diminished with chunk- or keyheader sizes.
 *
 * \returns Zero on success, TDERR_SPACE when no piece can be found, or
 * any of the errors of the underlying functions.
 *
 * \par NB:
 * This function assumes that the header will be written again
 * when this function returns successfully.
 */
int td_claim
  (td_t* td, int contiguous, unsigned* off, unsigned* size)
{
  int r = td_claim_(td, contiguous, off, size);
  if (r) {
    if (r == TDERR_SPACE && td->extend) {
      unsigned wanted = td->header.size;
      if ((wanted < *size) || (td->header.flags & TDFLG_SPARSE)) {
        wanted = *size;
      }
      CHECK(td_extend(td, wanted));
      CHECK(td_claim_(td, contiguous, off, size));
      return 0;
    }
  }
  return r;
}

#ifdef __cplusplus
}
#endif
