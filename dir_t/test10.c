#include <stdlib.h>
#include <stdio.h>
#include "td_private.h"

void debug_mem
  (void* _mem, unsigned int size);

static
char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

int main(int argc, char* argv[]) {
  td_t td;
  td_init_malloc(&td, TDFLG_ALLOCTDT|TDFLG_DUPLKEYS|TDFLG_EXTEND);
  {
    tdt_t key, value;
    int i;
    for (i=0; i<30; i++) {
      char keymem[3], valmem[3];
      keymem[0] = alphabet[i % 7];
      keymem[1] = alphabet[i % 13];
      keymem[2] = alphabet[i % 23];
      valmem[0] = alphabet[25 - (i % 7)];
      valmem[1] = alphabet[25 - (i % 13)];
      valmem[2] = alphabet[25 - (i % 23)];
      key = (tdt_t){ keymem, 3 };
      value = (tdt_t){ valmem, 3 };
      fprintf(stderr, "\nPutting %-.*s -> %-.*s\n", 3, keymem, 3, valmem);
      CHECK(td_put(&td, &key, &value, 0));
    }
  }
  int n = 0;
  while (1) {
    tdt_t key = { 0, 0 }, value = { 0, 0 };
    int r = td_pop(&td, &key, &value, 0);
    if (r == TDERR_NOTFOUND) {
      break;
    }
    fprintf(stderr, "Popping '%-.*s' -> '%-.*s'\n",
      key.size, (char*)(key.data), value.size, (char*)(value.data)
    );
    free(key.data);
    free(value.data);
    ++n;
  }
  if (n != 30) {
    fprintf(stderr, "Didn't pop 30 !\n");
    return ~0;
  }
  td_debug(&td);
  return 0;
}
