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
 * Convenience function to create an initialized tdt from a zero-terminated
 * string.
 *
 * \param[in] tdt Potentially NULL tdt structure pointer. When
 * non-NULL, this space will be initialized on return.
 * \param[in] string The string used to initialize the tdt.
 * Bear in mind that the memory isn't copied. If this pointer becomes
 * invalid during the lifetime of the tdt, you have a problem.
 *
 * \returns An initialized tdt.
 */
tdt_t tdt_string
  (tdt_t* tdt, char* string)
{
  if (tdt) {
    tdt->data = string;
    tdt->size = strlen(string);
    return *tdt;
  } else {
    return (tdt_t){ string, strlen(string) };
  }
}

#ifdef __cplusplus
}
#endif
