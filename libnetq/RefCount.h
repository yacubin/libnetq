/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_REFCOUNT_H
#define _LIBNETQ_REFCOUNT_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQRefCount {
  unsigned count;
} NQRefCount;

static inline void NQRefCount_init(NQRefCount* thiz)
{
  thiz->count = 1;
}

static inline void NQRefCount_ref(NQRefCount* thiz)
{
  thiz->count++;
}

#define NQRefCount_unref(thiz, func, ptr) \
  do {                      \
    --(thiz)->count;        \
    if ((thiz)->count == 0) \
      func(ptr);            \
  } while(0)

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_REFCOUNT_H */
