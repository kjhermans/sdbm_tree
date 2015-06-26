#include <stdio.h>
#include "td_private.h"

void debug_mem
  (void* _mem, unsigned int size);

#define MEMSIZE 1024

static
char mem[ MEMSIZE ];

int main(int argc, char* argv[]) {
  fprintf(stderr, "\n\nTesting one put\n\n");
  td_t td;
  tdt_t key = tdt_string(0, "foo"), value = tdt_string(0, "bar");
  td_init_mem(&td, 0, (void*)mem, MEMSIZE);
  CHECK(td_put(&td, &key, &value, 0));
  td_debug(&td);
  debug_mem(mem, 512);
  return 0;
}
