#include <stdlib.h>
#include <stdio.h>
#include "td_private.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static char* file = "/tmp/foo.db";

int main(int argc, char* argv[]) {

  int result = 0;

  td_t td;
  struct stat s;

  if (td_open(&td, file, TDFLG_EXTEND|TDFLG_CHECKSUM|TDFLG_SPARSE,
                  O_RDWR|O_CREAT|O_TRUNC,
                  0664)) {
    fprintf(stderr, "Could not open file '%s'.\n", file);
    return -1;
  }

  if (stat(file, &s)) {
    fprintf(stderr, "Could not stat '%s'.\n", file);
    return -1;
  }

  if (s.st_size > sizeof(struct td_header)) {
    fprintf(stderr, "File is larger than assumed at init.\n");
    return -1;
  }

  {
    char* name = "foo";
    char* value = "bar";
    tdt_t dbkey = { name, strlen(name) };
    tdt_t dbval = { value, strlen(value) };

    if (td_put(&td, &dbkey, &dbval, TDFLG_SPARSE)) {
      fprintf(stderr, "Put error.\n");
      return -1;
    }
  }

  if (stat(file, &s)) {
    fprintf(stderr, "Could not stat '%s'.\n", file);
    return -1;
  }

  if (s.st_size > sizeof(struct td_header) + 44) {
    fprintf(stderr, "File is larger than assumed after put.\n");
    return -1;
  }

  return result;
}
