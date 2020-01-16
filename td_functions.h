/*
** Copyright 2014 K.J. Hermans (kees-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifndef _TD_FUNCTIONS_H_
#define _TD_FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

extern
int td_read_keydata
  (
    td_t* td,
    unsigned keyoff,
    struct keyhead* keyhead,
    tdt_t* key,
    unsigned flags
  );

extern
int tdc_nxt
  (tdc_t* tdc, tdt_t* key, tdt_t* value, unsigned flags);

extern
int td_debug_node
  (td_t* td, unsigned node_offset, unsigned level);

extern
int td_push
  (td_t* td, const tdt_t* value);

extern
int td_yield
  (td_t* td, unsigned off);

extern
int tdc_get_locked
  (tdc_t* tdc, tdt_t* key, tdt_t* value, unsigned flags);

extern
int td_del
  (td_t* td, const tdt_t* key, tdt_t* value, unsigned flags);

extern
int td_rebalance_node
  (
    td_t* td,
    unsigned parentptr,
    struct keyhead* parentnode,
    unsigned ptr,
    unsigned* weight
  );

extern
int td_rotate_right
  (
    td_t* td,
    unsigned parentptr,
    struct keyhead* parent,
    unsigned ptr,
    struct keyhead* node
  );

extern
int td_extend
  (td_t* td, unsigned wanted);

extern
int td_yield_set_zero
  (td_t* td, unsigned off);

extern
int td_defrag
  (td_t* td);

extern
int td_write_uint
  (td_t* td, unsigned off, unsigned value);

extern
int td_get_stream
  (td_t* td, const tdt_t* key, int fd, unsigned flags);

extern
int td_init_fd
  (td_t* td, unsigned flags, int fd);

extern
int td_claim
  (td_t* td, int contiguous, unsigned* off, unsigned* size);

extern
int td_read_chunkhead
  (td_t* td, unsigned off, struct chunkhead* chunkhead);

extern
int td_put_key
  (td_t* td, const tdt_t* key, const unsigned valueptr, unsigned flags);

extern
int td_read_value
  (td_t* td, unsigned ptr, tdt_t* value, unsigned flags);

extern
int td_write_header
  (td_t* td);

extern
int tdc_debug
  (tdc_t* cursor);

extern
int td_pop
  (td_t* td, tdt_t* key, tdt_t* value, unsigned flags);

extern
int td_checksum_verify
  (void* mem, unsigned length, unsigned checksum);

extern
int td_rebalance
  (td_t* td);

extern
int td_put_new
  (
    td_t*         td,
    const tdt_t*  key,
    unsigned      valueptr,
    unsigned*     keyptr,
    unsigned      previous,
    unsigned      next,
    unsigned      flags
  );

extern
int tdc_rpl
  (tdc_t* tdc, const tdt_t* value, unsigned flags);

extern
void td_exit
  (td_t* td);

extern
int td_debug_path
  (struct path* path);

extern
int td_iterate_to_min
  (td_t* td, struct path* path);

extern
int td_write_keyhead
  (td_t* td, unsigned off, const struct keyhead* keyhead);

extern
int td_store_value
  (
    td_t* td,
    const tdt_t* value,
    unsigned refcount,
    unsigned* off,
    unsigned flags
  );

extern
int td_init_mmap
  (td_t* td, unsigned flags, int fd, unsigned off, unsigned siz);

extern
int tdc_init
  (td_t* td, tdc_t* tdc);

extern
int td_del_pathhead
  (td_t* td, struct path* path, tdt_t* value, unsigned flags);

extern
int td_init_mem
  (td_t* td, unsigned flags, void* mem, unsigned size);

extern
int td_read_uint
  (td_t* td, unsigned off, unsigned* data);

extern
int td_debug
  (td_t* td);

extern
int td_iterate_to_max
  (td_t* td, struct path* path);

extern
int td_init2
  (td_t* td, const char* ident, unsigned align, unsigned flags);

extern
int td_put_keys
  (td_t* td, const tdt_t* value, unsigned flags, unsigned nkeys, ...);

extern
int td_yield_all
  (td_t* td, unsigned ptr);

extern
int td_init
  (td_t* td);

extern
int td_compare
  (td_t* td, const tdt_t* key1, const tdt_t* key2, int partial, void* arg);

extern
int tdc_last
  (tdc_t* tdc);

extern
int td_iterate_to_key
  (td_t* td, struct path* path, const tdt_t* key, int partial);

extern
int td_put_replace
  (
    td_t*            td,
    const unsigned   valueptr,
    const unsigned   keyptr,
    struct keyhead*  keyhead
  );

extern
int td_store_value_stream
  (
    td_t* td,
    int fd,
    unsigned refcount,
    unsigned* off,
    unsigned flags
  );

extern
int tdc_prv
  (tdc_t* tdc, tdt_t* key, tdt_t* value, unsigned flags);

extern
int td_path_next
  (td_t* td, struct path* path);

extern
int td_path_previous
  (td_t* td, struct path* path);

extern
int tdc_get_stream
  (tdc_t* tdc, tdt_t* key, int fd, unsigned flags);

extern
int td_read_value_stream
  (td_t* td, unsigned ptr, int fd, unsigned flags);

extern
int td_lock
  (td_t* td);

extern
int td_get
  (td_t* td, const tdt_t* key, tdt_t* value, unsigned flags);

extern
int td_write
  (td_t* td, unsigned off, const void* buf, unsigned size);

extern
int td_put_stream_locked_key
  (td_t* td, const tdt_t* key, int fd, unsigned flags);

extern
void td_qsort
  (unsigned* list, unsigned length);

extern
int td_put
  (td_t* td, const tdt_t* key, const tdt_t* value, unsigned flags);

extern
int td_verify
  (td_t* td, int fix);

extern
int td_iterate
  (td_t* td, struct path* path, const tdt_t* key, int partial);

extern
void td_checksum_create
  (void* mem, unsigned length, unsigned* checksum);

extern
int td_put_stream
  (td_t* td, const tdt_t* key, int fd, unsigned flags);

extern
int td_rotate_left
  (
    td_t* td,
    unsigned parentptr,
    struct keyhead* parent,
    unsigned ptr,
    struct keyhead* node
  );

extern
int tdc_first
  (tdc_t* tdc);

extern
int td_write_chunkhead
  (td_t* td, unsigned off, const struct chunkhead* chunkhead);

extern
int td_unlink_value
  (td_t* td, unsigned valueptr);

extern
int td_ar_unpack
  (td_t* td);

extern
int td_init_chunk
  (td_t* td, unsigned flags, int fd, unsigned off, unsigned size);

extern
int td_unlock
  (td_t* td);

extern
int tdc_get
  (tdc_t* tdc, tdt_t* key, tdt_t* value, unsigned flags);

extern
int td_read
  (td_t* td, unsigned off, void* buf, unsigned size);

extern
int td_read_keyhead
  (td_t* td, unsigned off, struct keyhead* keyhead);

extern
int td_ar_scan
  (char* path, td_t* td);

extern
int td_wipe
  (td_t* td);

extern
int td_put_locked_key
  (td_t* td, const tdt_t* key, const tdt_t* value, unsigned flags);

extern
int td_rmw
  (
    td_t* td,
    const tdt_t* key,
    tdt_t* value,
    unsigned flags,
    rmwfnc_t callback,
    void* arg
  );

extern
int td_read_header
  (td_t* td);

extern
int td_ar_list
  (td_t* td, int longform);

extern
int td_init_malloc
  (td_t* td, unsigned flags);

extern
int td_iterate_compare
  (td_t* td, struct stackelt* elt, const tdt_t* key, int partial);

extern
void td_assoc_admin
  (td_t* td, td_t* admin, unsigned assocflags);

extern
int tdc_mov
  (tdc_t* tdc, const tdt_t* key, unsigned flags);

extern
int td_open
  (td_t* td, char* path, unsigned tdflags, unsigned openflags, unsigned mode);

#ifdef __cplusplus
}
#endif

#endif
