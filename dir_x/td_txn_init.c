/**
 * Copyright 2020 K.J. Hermans (kees.jan.hermans@gmail.com)
 * This code is part of simpledbm, an API to a dbm on a finite resource.
 * License: BSD
 *
 * \brief BRIEF
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"
#include "td_txn.h"

/**
 * \ingroup txn
 *
 * \brief Initializes a transation structure.
 *
 * Transactions are used to perform multiple step operations on the
 * database, as if they were all executed at once.
 *
 * \param td  Database pointer, already associated with an admin db.
 * \param txn The uninitialized transaction structure.
 *
 * \returns Zero on success, non-zero on error, when:
 * - the database is not associated with an admin database
 *   or the admin database is not of the proper kind.
 */
int td_txn_init
  (td_t* td, td_txn_t* txn)
{
  if (td->admin.db == NULL || !(td->admin.flags & TD_ADMIN_TXN)) {
    //.. return error
  }
  memset(txn, 0, sizeof(*txn));
  txn->td = td;
  return 0;
}

#ifdef __cplusplus
}
#endif
