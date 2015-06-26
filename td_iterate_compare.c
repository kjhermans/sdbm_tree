/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/**
 * \ingroup btree_private
 * Compares a path (stack-) element to a given key.
 * Leaves the result of the comparison in the element itself.
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param elt Element containing a pointer to the current key in the path.
 * \param key Non-NULL pointer to a potentially uninitialized tdt.
 * \param partial Boolean. Whether or not matches are allowed to be partial.
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_iterate_compare
  (td_t* td, struct stackelt* elt, const tdt_t* key, int partial)
{
  uchar keydata[ elt->keyhead.size - sizeof(struct keyhead) ];
  tdt_t cmpkey = { keydata, sizeof(keydata) };
  CHECK(td_read_keydata(td, elt->ptr, &(elt->keyhead), &cmpkey, 0));
  elt->cmp = td->compare(td, key, &cmpkey, partial, td->cmparg);
  if (elt->cmp < 0) {
    elt->cmp = -1;
  } else if (elt->cmp > 0) {
    elt->cmp = 1;
  }
  return 0;
}
