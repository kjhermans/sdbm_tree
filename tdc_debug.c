/*
** Copyright 2015 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/**
 * \ingroup btree_private
 *
 * Debugs the data structure of a cursor to stderr.
 *
 * \param cursor Non-NULL pointer to an initialized cursor.
 *
 * \returns Zero on success, or any other error on failure.
 */
int tdc_debug
  (tdc_t* cursor)
{
  fprintf(stderr, "Cursor: SCN=%u\n", cursor->scn);
  return td_debug_path(&(cursor->path));
}

#ifdef __cplusplus
}
#endif
