#include <stdio.h>
#include "td_private.h"

void debug_mem
  (void* _mem, unsigned int size);

#define MEMSIZE 1024

static
char area[ MEMSIZE ];

int main(int argc, char* argv[]) {
  fprintf(stderr, "\n\nTesting one put and one get\n\n");
  td_t td;
  tdt_t key = tdt_string(0, "foo"), value = tdt_string(0, "bar");
  char mem[100];
  tdt_t rvalue = { mem, 100 };
  td_init_mem(&td, 0, (void*)area, MEMSIZE);
  CHECK(td_debug(&td));
  fprintf(stderr, "\n\nDo put\n");
  CHECK(td_put(&td, &key, &value, 0));
  CHECK(td_debug(&td));
  fprintf(stderr, "\n\nDo get\n");
  CHECK(td_get(&td, &key, &rvalue, 0));
  CHECK(td_debug(&td));
  fprintf(stderr, "Received '%-.*s'\n", rvalue.size, (char*)(rvalue.data));
  if (rvalue.size == 3 && memcmp(rvalue.data, "bar", 3) == 0) {
    return 0;
  } else {
    fprintf(stderr, "FAILED.\n");
    return ~0;
  }
}
