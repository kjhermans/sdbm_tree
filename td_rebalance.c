/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"
#include <math.h>

/**
 * Just having this function means that we don't have to link with libmath.
 */
static inline
unsigned td_log2
  (unsigned n)
{
  unsigned r = 0;
  while (n) {
    n >>= 1;
    ++r;
  }
  return r;
}

/**
 * \ingroup btree_private
 *
 * Rebalances the tree a little bit:
 * For every imbalance, the top is shifted one in the right direction,
 * even when the imbalance is bigger.
 *
 * \param[in] td Non-NULL pointer to an initialized btree structure.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_rebalance
  (td_t* td)
{
  unsigned weight;
  double depthfactor = (td_log2(td->header.nentries) * TD_DEPTHEXCESS);
  if (td->header.top == 0 ||
      td->header.nentries < 2 ||
      (double)(td->maxdepth) < depthfactor)
  {
    return 0;
  }
  CHECK(td_rebalance_node(td, 0, 0, td->header.top, &weight));
  td->maxdepth = (int)(depthfactor / 2);
  return 0;
}

#ifdef __cplusplus
}
#endif
