#include <stdio.h>
#include "td_private.h"

void debug_mem
  (void* _mem, unsigned int size);

#define MEMSIZE 1024

static
char mem[ MEMSIZE ];

int main(int argc, char* argv[]) {
  fprintf(stderr, "\n\nTesting init\n\n");
  td_t td;
  td_init_mem(&td, TDFLG_SPARSE, (void*)mem, MEMSIZE);
  debug_mem(mem, td.header.size);
  return 0;
}
