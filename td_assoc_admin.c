/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/**
 * \ingroup btree
 *
 */
void td_assoc_admin
  (td_t* td, td_t* admin, unsigned assocflags)
{
  td->admin.db = admin;
  td->admin.flags = assocflags;
}

#ifdef __cplusplus
}
#endif
