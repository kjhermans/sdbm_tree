/*
** Copyright 2015 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "td_private.h"

/** 
 * \ingroup btree_private 
 *
 * Sorts a list of unsigned integers.
 *
 * \param list Non-NULL pointer to a list of unsigned integers.
 * \param length Number of elements in the list.
 */ 
void td_qsort
  (unsigned* list, unsigned length)
{
  if (length <= 1) {
    return;
  } else if (length == 2) {
    if (list[0] > list[1]) {
      unsigned tmp = list[0];
      list[0] = list[1];
      list[1] = tmp;
    }
    return;
  } else {
    unsigned pivotpos = length / 2;
    unsigned pivotval = list[pivotpos];
    unsigned smaller[ length ], smallerlength = 0;
    unsigned equal[ length ], equallength = 0;
    unsigned bigger[ length ], biggerlength = 0;
    unsigned i, l;
    for (i=0; i < length; i++) {
      if (i == pivotpos) {
        equal[equallength++] = pivotval;
      } else {
        if (list[i] < pivotval) {
          smaller[smallerlength++] = list[i];
        } else if (list[i] > pivotval) {
          bigger[biggerlength++] = list[i];
        } else {
          equal[equallength++] = pivotval;
        }
      }
    }
    td_qsort(smaller, smallerlength);
    td_qsort(bigger, biggerlength);
    for (i=0, l=0; i<smallerlength; i++, l++) {
      list[l] = smaller[i];
    }
    for (i=0; i<equallength; i++, l++) {
      list[l] = equal[i];
    }
    for (i=0; i<biggerlength; i++, l++) {
      list[l] = bigger[i];
    }
  }
}

#ifdef __cplusplus
}
#endif
