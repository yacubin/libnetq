/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_STUB_MUTEX_H
#define _LIBNETQ_SYNC_STUB_MUTEX_H

#include <libnetq/ErrorCode.h>

typedef struct NQMutex NQMutex;
struct NQMutex {
  int dummy;
};

#define NQ_MUTEX_DEFINE(mutexname) \
  NQMutex mutexname = { 0 }

static inline int NQMutex_init(NQMutex* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return NQ_ENOTSUP;
}

static inline int NQMutex_finalize(NQMutex* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return NQ_ENOTSUP;
}

static inline int NQMutex_lock(NQMutex* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return NQ_ENOTSUP;
}

static inline bool NQMutex_trylock(NQMutex* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return false;
}

static inline int NQMutex_unlock(NQMutex* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return NQ_ENOTSUP;
}

#endif /* _LIBNETQ_SYNC_STUB_MUTEX_H */
