#ifndef _TD_AR_H_
#define _TD_AR_H_

#include <stdint.h>

#define TDAR_IRUSR                      00400
#define TDAR_IWUSR                      00200
#define TDAR_IXUSR                      00100

#define TDAR_IRGRP                      00040
#define TDAR_IWGRP                      00020
#define TDAR_IXGRP                      00010

#define TDAR_IROTH                      00004
#define TDAR_IWOTH                      00002
#define TDAR_IXOTH                      00001

#define TDAR_TYP_FILE                   1
#define TDAR_TYP_DIR                    2
#define TDAR_TYP_SYMLINK                3

#define TDAR_MAGIC                      0x74647200 /* spells 'tdr\0' */

typedef struct
{
  uint32_t magic;
  uint16_t type;
  uint16_t bits;
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
