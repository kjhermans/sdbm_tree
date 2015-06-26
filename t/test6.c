#include <stdio.h>
#include "td_private.h"

void debug_mem
  (void* _mem, unsigned int size);

#define MEMSIZE 1024

static
char area[ MEMSIZE ];

int main(int argc, char* argv[]) {
  fprintf(stderr, "\n\nTesting a put, a get, a del and one more put\n\n");
  td_t td;
  tdt_t key1 = tdt_string(0, "foo"), value1 = tdt_string(0, "bar");
  tdt_t key2 = tdt_string(0, "puns"), value2 = tdt_string(0, "achs");
  char mem[100];
  tdt_t rvalue = { mem, 100 };
  memset(area, 0, MEMSIZE);
  td_init_mem(&td, 0, (void*)area, MEMSIZE);
  fprintf(stderr, "\n\nEmpty:\n");
  td_debug(&td);
  fprintf(stderr, "\n\nPut:\n");
  CHECK(td_put(&td, &key1, &value1, 0));
  td_debug(&td);
  fprintf(stderr, "\n\nGet:\n");
  CHECK(td_get(&td, &key1, &rvalue, 0));
  fprintf(stderr, "Received '%-.*s'\n", rvalue.size, (char*)(rvalue.data));
  td_debug(&td);
  fprintf(stderr, "\n\nDel:\n");
  CHECK(td_del(&td, &key1, &rvalue, 0));
  td_debug(&td);
  fprintf(stderr, "\n\nGet:\n");
  if (td_get(&td, &key1, &rvalue, 0) != TDERR_NOTFOUND) {
    return ~0;
  }
  fprintf(stderr, "\n\nPut:\n");
  CHECK(td_put(&td, &key2, &value2, 0));
  td_debug(&td);
  debug_mem(area, 512);
  return 0;
}
