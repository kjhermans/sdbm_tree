/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a btree on a finite resource.
** License: BSD
*/

#ifndef _TD_TYPES_H_
#define _TD_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct td  td_t;
typedef struct tdt tdt_t;
typedef struct tdc tdc_t;

struct tdt
  {
    void*             data;      /* Memory */
    unsigned          size;      /* Its size */
  };

/**
 * A binary tree is theoretically capable of storing 2 ^ TD_MAXDEPTH - 1 nodes.
 */
#define TD_MAXDEPTH                     128
#define TD_DEPTHEXCESS                  2
#define TD_MAXPATH                      (TD_DEPTHEXCESS * TD_MAXDEPTH)

struct keyhead
  {
    unsigned          value;     /* Pointer to value's first chunk */
    unsigned          size;      /* This chunk's size, including this header */
    unsigned          checksum;  /* Optional checksum of this chunk */
    unsigned          next;      /* Pointer to keyhead with higher keys */
    unsigned          previous;  /* Pointer to keyhead with lower keys */
  };

#define TD_KEYHEAD_NULL (struct keyhead){ 0, 0, 0, 0, 0 }

struct stackelt
  {
    unsigned          ptr;
    struct keyhead    keyhead;
    int               cmp;
  };

struct path
  {
    struct stackelt   stack[ TD_MAXPATH ];
    struct stackelt*  head;
    unsigned          size;
  };

#define TD_PATH_INIT(__ptr) ({                                              \
  struct path __p;                                                          \
  memset(&__p, 0, sizeof(__p));                                             \
  __p.stack[0].ptr=(__ptr);                                                 \
  __p.head=&(__p.stack[0]);                                                 \
  __p.size=1;                                                               \
  __p;                                                                      \
})

#ifdef _TD_PRIVATE_

//struct valuehead
//  {
//    unsigned          next;      /* Pointer to next chunk, or next key */
//    unsigned          size;      /* This chunk's size, including header */
//    unsigned          checksum;  /* Optional checksum of this chunk */
//    unsigned          refcount;  /* Number of keys pointing here */
//  };

struct chunkhead
  {
    unsigned          next;      /* Pointer to next chunk, or next key */
    unsigned          size;      /* This chunk's size, including these 2 ints */
    unsigned          checksum;  /* Optional checksum of this chunk */
    unsigned          refcount;  /* Number of keys pointing here */
  };

#define TD_PATH_DEBUG(__p)                                                  \
  {                                                                         \
    fprintf(stderr, "PATH size=%u\n", (__p)->size);                         \
    unsigned i; for (i=0; i< (__p)->size; i++) {                            \
      fprintf(stderr, "PATH %u; ptr=@%u, \n", i, (__p)->stack[i].ptr);      \
    }                                                                       \
  }

#define TD_PATH_VALID(__p)                                                  \
  if ((__p)->size == 0 || (__p)->size >= TD_MAXDEPTH) {                     \
    TD_PATH_DEBUG(__p);                                                     \
    return TDERR_STRUCT;                                                    \
  } else {                                                                  \
    (__p)->head = &((__p)->stack[ (__p)->size - 1 ]);                       \
  }

#endif // _TD_PRIVATE_

struct td_header
  {
    char              magic[4];  /* Spells: { 't', 'd', 'i', '\0' } */
    unsigned          flags;     /* Tweak behaviour */
    unsigned          size;      /* Our working limit */
    unsigned          align;     /* If you're on a block device, normally 1 */
    unsigned          top;       /* Initial pointer list offset */
    unsigned          level;     /* The maximum depth of the tree */
    unsigned          off_e;     /* Where our empty list starts */
    unsigned          scn;       /* Every change causes this to increase */

    /* Statistics */
    unsigned          nentries;  /* The total amount of entries */
    unsigned          nempties;  /* Amount of empty chunks */
    unsigned          nchunks;   /* Amount of chunks */
  };

struct td
  {

    struct td_header  header;
    unsigned          id;
    int               locked;
 
    /* Callbacks to implement locking; not obligatory */
    int             (*lock)(td_t*,int,void*);
    void*             lockarg;

    /* Callbacks to implement retrieval and storage; obligatory */
    int             (*read)(td_t*, unsigned, void*, unsigned, void*);
    int             (*write)(td_t*, unsigned, const void*, unsigned, void*);
    void            (*close)(td_t*);
    void*             ioarg;

    /* Callback to implement (a different form of) sorting; not obligatory */
    int             (*compare)(td_t*,const tdt_t*,const tdt_t*,int,void*);
    void*             cmparg;

    /* Callback to implement extension of the working area; not obligatory */
    unsigned        (*extend)(td_t*,unsigned,void*);
    void*             extendarg;

    /* Callback to implement (m)allocating memory in keys and values; not ob. */
    void*           (*realloc)(td_t*,void*,unsigned,void*);
    void*             reallocarg;

    unsigned          maxdepth;

    struct {
      int               fd;
      void*             mem;
      unsigned          offset;
    }                 resource;

  };
  
/* Cursor */
struct tdc
  {
    td_t*             td;
    unsigned          scn;
    struct path       path;
  };

#ifdef __cplusplus
}
#endif

#endif
