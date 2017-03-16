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
      keymem[0] = alphabet[25 - (i % 7)];
      keymem[1] = alphabet[25 - (i % 13)];
      keymem[2] = alphabet[25 - (i % 23)];
      valmem[0] = alphabet[(i % 7)];
      valmem[1] = alphabet[(i % 13)];
      valmem[2] = alphabet[(i % 23)];
      key = (tdt_t){ keymem, 3 };
      value = (tdt_t){ valmem, 3 };
      fprintf(stderr, "Putting %-.*s -> %-.*s\n", 3, keymem, 3, valmem);
      CHECK(td_put(&td, &key, &value, 0));
    }
  }
  {
    tdc_t tdc;
    tdc_init(&td, &tdc);
    while (1) {
      tdt_t key = (tdt_t){ 0, 0 }, value = (tdt_t){ 0, 0 };
      int r = tdc_nxt(&tdc, &key, &value, 0);
      if (r == 0) {
        fprintf(stderr, "Cursor iteration; %-.*s -> %-.*s\n",
          key.size, (char*)(key.data), value.size, (char*)(value.data)
        );
        free(key.data);
        free(value.data);
      } else if (r == TDERR_NOTFOUND) {
        break;
      } else {
        fprintf(stderr, "Cursor iteration; %d\n", r);
        return ~0;
      }
    }
  }
  {
    tdc_t tdc;
    tdc_init(&td, &tdc);
    tdt_t key = tdt_string(0, "v"), value;
    fprintf(stderr, "Moving\n");
    if (tdc_mov(&tdc, &key, TDCFLG_PARTIAL)) {
      fprintf(stderr, "Couldn't move cursor.\n");
      return ~0;
    }
    while (1) {
      key = (tdt_t){ 0, 0 }, value = (tdt_t){ 0, 0 };
      int r = tdc_nxt(&tdc, &key, &value, 0);
      if (r == 0) {
        fprintf(stderr, "Cursor iteration; %-.*s -> %-.*s\n",
          key.size, (char*)(key.data), value.size, (char*)(value.data)
        );
        free(key.data);
        free(value.data);
      } else if (r == TDERR_NOTFOUND) {
        break;
      } else {
        fprintf(stderr, "Cursor iteration; %d\n", r);
        return ~0;
      }
    }
  }
  return 0;
}
