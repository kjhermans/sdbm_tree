#include <stdio.h>
#include "td_private.h"

void debug_mem
  (void* _mem, unsigned int size);

#define MEMSIZE 1024

static
char area[ MEMSIZE ];

int main(int argc, char* argv[]) {
  fprintf(stderr, "\n\nTesting two puts, a get, a del and two gets\n\n");
  td_t td;
  tdt_t key1 = tdt_string(0, "foo"), value1 = tdt_string(0, "bar");
  tdt_t key2 = tdt_string(0, "pun"), value2 = tdt_string(0, "ach");
  char mem[100];
  tdt_t rvalue = { mem, 100 };
  memset(area, 0, MEMSIZE);
  td_init_mem(&td, 0, (void*)area, MEMSIZE);
  CHECK(td_put(&td, &key1, &value1, 0));
  CHECK(td_put(&td, &key2, &value2, 0));
  td_debug(&td);
  debug_mem(area, 512);
  CHECK(td_get(&td, &key1, &rvalue, TDFLG_EXACT));
  fprintf(stderr, "Received '%-.*s'\n", rvalue.size, (char*)(rvalue.data));
  CHECK(td_del(&td, &key1, &rvalue, 0));
  td_debug(&td);
  debug_mem(area, 512);
  if (td_get(&td, &key1, &rvalue, TDFLG_EXACT) != TDERR_NOTFOUND) {
    return ~0;
  }
  CHECK(td_get(&td, &key2, &rvalue, TDFLG_EXACT));
  fprintf(stderr, "Received '%-.*s'\n", rvalue.size, (char*)(rvalue.data));
  return 0;
}
