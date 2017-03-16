#include <stdlib.h>
#include <stdio.h>
#include "td_private.h"

void debug_mem
  (void* _mem, unsigned int size);

static
char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

int main(int argc, char* argv[]) {

  int result = 0;

  struct testcase {
    char movkey[ 10 ];
    unsigned flags;
    int outcome;
    char getkey[ 10 ];
  };

  struct testcase cases[] = {
//    { "xxx", 0, 0, "xxx" },
//    { "xx", 0, 0, "xxx" },
//    { "x", 0, 0, "xxx" },
    { "xxx", TDCFLG_PARTIAL, 0, "xxx" },
    { "xx", TDCFLG_PARTIAL, 0, "xxx" },
    { "x", TDCFLG_PARTIAL, 0, "xpz" },
    { "xxx", TDCFLG_EXACT, 0, "xxx" },
    { "xx", TDCFLG_EXACT, 1, "xxx" },
    { "x", TDCFLG_EXACT, 1, "xxx" },
    { "xxx", TDCFLG_PARTIAL|TDCFLG_EXACT, 0, "xxx" },
    { "xx", TDCFLG_PARTIAL|TDCFLG_EXACT, 0, "xxx" },
    { "x", TDCFLG_PARTIAL|TDCFLG_EXACT, 0, "xpz" }
  };

  unsigned ncases = 9;

  struct testcase* curcase = cases;

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

  td_debug(&td);

  while (ncases--) {
    char keymem[ 10 ], valuemem[ 10 ];
    tdt_t key, value;
    tdc_t tdc;

    fprintf(stderr, "\n\n\n----\n"
      "CASE: movkey='%s', flags=%s|%s, outcome=%d, getkey='%s'\n",
      curcase->movkey,
      ((curcase->flags & TDCFLG_PARTIAL)?"PARTIAL":""),
      ((curcase->flags & TDCFLG_EXACT)?"EXACT":""),
      curcase->outcome,
      curcase->getkey
    );

    tdc_init(&td, &tdc);
    memcpy(keymem, curcase->movkey, strlen(curcase->movkey));
    key.data = keymem;
    key.size = strlen(curcase->movkey);
    int r = tdc_mov(&tdc, &key, curcase->flags);
    td_debug_path(&(tdc.path));
    if (r != curcase->outcome) {
      fprintf(stderr, "Wrong outcome %d\n", r);
      result = 3;
      goto NEXT;
    }
    if (r) {
      goto SUCCESS;
    }
    key.size = sizeof(keymem);
    value.data = valuemem;
    value.size = sizeof(valuemem);
    r = tdc_get(&tdc, &key, &value, curcase->flags);
    if (r) {
      fprintf(stderr, "Get returns error %d\n", r);
      result = 2;
      goto NEXT;
    }
    if (key.size != strlen(curcase->getkey) ||
        memcmp(key.data, curcase->getkey, strlen(curcase->getkey)))
    {
      fprintf(stderr, "Wrong key at cursor '%-.*s' (expected '%s')\n",
        key.size, (char*)(key.data), curcase->getkey
      );
      result = 1;
      goto NEXT;
    }
SUCCESS:
    fprintf(stderr, "TEST SUCCEEDED.\n");
NEXT:
    ++curcase;
  }
  return result;
}
