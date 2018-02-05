#ifndef _TD_AR_H_
#define _TD_AR_H_

#include <stdint.h>

typedef struct
{
  uint32_t magic;
  uint32_t flags;
  uint32_t uid;
  uint32_t gid;
  uint64_t size;
  uint64_t ctime;
  uint64_t mtime;
  uint64_t atime;
}
__attribute__((packed))
td_ar_stat_t;

#define TD_AR_PATH_MAX                  2048
#define TD_AR_KEY_MAX                   (TD_AR_PATH_MAX + sizeof(td_ar_stat_t))

#endif // !_TD_AR_H_
