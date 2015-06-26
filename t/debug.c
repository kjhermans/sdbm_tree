#include <stdio.h>
#include <ctype.h>

void debug_mem
  (void* _mem, unsigned int size)
{
  char* mem = _mem;
  char print[16];
  unsigned int c = 0;
  *print = 0;
  if (size == 0) {
    fprintf(stderr, "Memory being debugged is of zero size.\n");
    return;
  }
  fprintf(stderr, "%.6d  ", 0);
  while (size--) {
    unsigned char byte = *mem++;
    fprintf(stderr, "%.2x ", byte);
    print[c % 16] = (isprint(byte) ? byte : '.');
    if ((++c % 16) == 0) {
      fprintf(stderr, "       %-.*s\n%.6x  ", 16, print, c);
      *print = 0;
    }
  }
  while (c % 16) {
    print[c % 16] = ' ';
    c++;
    fprintf(stderr, "   ");
  }
  fprintf(stderr, "       %-.*s\n", 16, print);
}
