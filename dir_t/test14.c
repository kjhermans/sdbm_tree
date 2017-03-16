#include <stdio.h>
#include "td_private.h"

void debug_mem
  (void* _mem, unsigned int size);

#define MEMSIZE 1024

static
char area[ MEMSIZE ];

int main(int argc, char* argv[]) {
  fprintf(stderr, "\n\nTesting two puts, two gets and two dels\n\n");
  td_t td;
  tdt_t key1 = tdt_string(0, "foo"), value1 = tdt_string(0, "bar");
  tdt_t key2 = tdt_string(0, "foo"), value2 = tdt_string(0, "ach");
  tdt_t key3 = tdt_string(0, "jaaa"), value3 = tdt_string(0, "nounou");
  char mem[100];
  tdt_t rvalue = { mem, 100 };
  memset(area, 0, MEMSIZE);
  td_init_mem(&td, TDFLG_DUPLKEYS, (void*)area, MEMSIZE);
  fprintf(stderr, "Put 1\n");
  CHECK(td_put(&td, &key1, &value1, 0));
  td_debug(&td);
  fprintf(stderr, "Put 2\n");
  CHECK(td_put(&td, &key2, &value2, 0));
  debug_mem(area, 512);
  td_debug(&td);
  fprintf(stderr, "Put 3\n");
  CHECK(td_put(&td, &key3, &value3, 0));
  debug_mem(area, 512);
  td_debug(&td);
  fprintf(stderr, "Get 1\n");
  rvalue.size = sizeof(mem);
  CHECK(td_get(&td, &key1, &rvalue, 0));
  fprintf(stderr, "Received '%-.*s'\n", rvalue.size, (char*)(rvalue.data));
  fprintf(stderr, "Get 2\n");
  rvalue.size = sizeof(mem);
  CHECK(td_get(&td, &key2, &rvalue, 0));
  fprintf(stderr, "Received '%-.*s'\n", rvalue.size, (char*)(rvalue.data));
  fprintf(stderr, "Get 3\n");
  rvalue.size = sizeof(mem);
  CHECK(td_get(&td, &key3, &rvalue, 0));
  fprintf(stderr, "Received '%-.*s'\n", rvalue.size, (char*)(rvalue.data));
  fprintf(stderr, "Del foo\n");
  rvalue.size = sizeof(mem);
  td_debug(&td);
  CHECK(td_del(&td, &key1, &rvalue, 0));
  fprintf(stderr, "Received '%-.*s'\n", rvalue.size, (char*)(rvalue.data));
  fprintf(stderr, "Del foo\n");
  rvalue.size = sizeof(mem);
  td_debug(&td);
  CHECK(td_del(&td, &key2, &rvalue, 0));
  fprintf(stderr, "Received '%-.*s'\n", rvalue.size, (char*)(rvalue.data));
  return 0;
}
