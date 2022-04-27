/*
** Copyright 2019 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

static
int td_increase_key
  (tdt_t* key)
{
  unsigned i = key->size;
  unsigned char* keydata = key->data;

  while (i > 0) {
    --i;
    if ((keydata[ i ])++ != 0) {
      return 0;
    }
  }
  return TDERR_OVERFLOW;
}

static
int td_push_locked
  (td_t* td, const tdt_t* value)
{
  unsigned char keydata[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  tdt_t key = { keydata, sizeof(keydata) };
  unsigned valueptr, keyptr;

  if (td->header.top == 0) {
    CHECK(td_store_value(td, value, 1, 1, &valueptr, 0));
    CHECK(td_put_new(td, &key, valueptr, &keyptr, 0, 0, 0));
    td->header.top = keyptr;
  } else {
    struct path path = TD_PATH_INIT(td->header.top);

    CHECK(td_iterate_to_max(td, &path));
    CHECK(td_read_keydata(td, path.head->ptr, &(path.head->keyhead), &key, 0));
    CHECK(td_increase_key(&key));
    CHECK(td_store_value(td, value, 1, 1, &valueptr, 0));
    CHECK(td_put_new(td, &key, valueptr, &keyptr, path.head->ptr, 0, 0));
  }
  CHECK(td_write_header(td));
  //CHECK(td_rebalance(td));
  return 0;
}

/**
 * \ingroup btree
 *
 * Pushes a value into the db, creating a (time) ordered list.
 * It uses the underlying principles of a btree, but is still efficient:
 * the algorithm moves to the last item of the db, takes its key,
 * increases the value of it bitwise (so that, under a default comparison
 * callback, it would be considered bigger), and stores the value there.
 * If the db is empty, will create a key consisting of a single byte
 * with value zero.
 *
 * \param td
 * \param value
 */
int td_push
  (td_t* td, const tdt_t* value)
{
  int r;
  CHECK(td_lock(td));
  r = td_push_locked(td, value);
  CHECK(td_unlock(td));
  return r;
}

#ifdef __cplusplus
}
#endif
