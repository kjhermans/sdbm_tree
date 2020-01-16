/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <ctype.h>

#include "td_private.h"

static
int td_debug_buffer
  (td_t* td, unsigned off, unsigned size)
{
  unsigned char buf[ size ];
  unsigned i;
  CHECK(td_read(td, off, buf, size));
  for (i=0; i < size; i++) {
    if (isprint(buf[ i ])) {
      fprintf(stderr, "%c", buf[ i ]);
    } else {
      unsigned c = buf[ i ];
      fprintf(stderr, "\\x%.2x", c);
    }
  }
  return 0;
}

static
int td_debug_value
  (td_t* td, unsigned node_offset, unsigned level)
{
  struct chunkhead chunkhead;
  while (node_offset) {
    unsigned i;
    CHECK(td_read_chunkhead(td, node_offset, &chunkhead));
    for (i=0; i < level; i++) { fprintf(stderr, "  "); }
    fprintf(stderr, "  Value chunk @%u, size %u(%u), next @%u '",
      node_offset,
      chunkhead.size,
      (unsigned)(chunkhead.size - sizeof(chunkhead)),
      chunkhead.next
    );
    CHECK(
      td_debug_buffer(
        td, node_offset + sizeof(chunkhead), chunkhead.size - sizeof(chunkhead)
      )
    );
    fprintf(stderr, "'\n");
    node_offset = chunkhead.next;
  }
  return 0;
}

/**
 * \ingroup btree_private
 *
 * Debugs the contents and children of a btree node to stderr.
 *
 * \param td Non-NULL pointer to an initialized btree structure.
 * \param node_offset Offset within the btree's resource, of the node
 * to be debugged.
 * \param level Indentation level of the output.
 *
 * \returns Zero on success, or a TDERR_* value on error.
 */
int td_debug_node
  (td_t* td, unsigned node_offset, unsigned level)
{
  unsigned i;
  struct keyhead keyhead;
  if (node_offset == 0) {
    return 0;
  }
  CHECK(td_read_keyhead(td, node_offset, &keyhead));
  if (keyhead.previous) {
    td_debug_node(td, keyhead.previous, level + 1);
  }
  for (i=0; i < level; i++) { fprintf(stderr, "- "); }
  fprintf(stderr,
    "Key @%u, value @%u, previous @%u, next @%u, size %u(%u) '",
    node_offset, keyhead.value, keyhead.previous, keyhead.next,
    (unsigned)(keyhead.size - sizeof(keyhead)), keyhead.size
  );
  CHECK(
    td_debug_buffer(
      td,
      node_offset + sizeof(keyhead),
      keyhead.size - sizeof(keyhead)
    )
  );
  fprintf(stderr, "'\n");
  CHECK(td_debug_value(td, keyhead.value, level));
  if (keyhead.next) {
    td_debug_node(td, keyhead.next, level + 1);
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
