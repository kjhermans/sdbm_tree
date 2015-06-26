#include <stdlib.h>
#include <stdio.h>
#include "td_private.h"

void debug_mem
  (void* _mem, unsigned int size);

static
char* keys[] = {
  "cccc",
  "aabb05",
  "aaab",
  "aabb01",
  "aabb11",
  "aabb03",
  "aabb09",
  "aabb07",
  "aabb10",
  "cccc",
  "aabb06",
  "aabb02",
  "cccc",
  "aaaa",
  "cccc",
  "aabb04",
  "cccc",
  "aaaa",
  "aabb08",
  "cccc",
  NULL
};

int main(int argc, char* argv[]) {
  td_t td;
  td_init_malloc(&td, TDFLG_ALLOCTDT|TDFLG_DUPLKEYS|TDFLG_EXTEND);
  {
    tdt_t key, value;
    char** keystr = keys;
    while (*keystr) {
      char keymem[10], valmem[10];
      strcpy(keymem, *keystr);
      strcpy(valmem, *keystr);
      key = (tdt_t){ keymem, strlen(keymem) };
      value = (tdt_t){ valmem, strlen(keymem) };
      fprintf(stderr, "Putting %s -> %s\n", keymem, valmem);
      CHECK(td_put(&td, &key, &value, 0));
      ++keystr;
    }
  }
  td_debug(&td);
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
    tdt_t key = tdt_string(0, "aabb"), value;
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
