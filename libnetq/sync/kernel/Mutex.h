/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_MUTEXKERNEL_H
#define _LIBNETQ_SYNC_MUTEXKERNEL_H

#include <libnetq/Basic.h>

#ifdef NQ_OS_KERNEL

#include <linux/mutex.h>

typedef struct mutex NQMutex;
#define NQ_MUTEX_DEFINE(mutexname) DEFINE_MUTEX(mutexname)

static inline int NQMutex_init(NQMutex* thiz)
{
  mutex_init(thiz);
  return 0;
}

static inline int NQMutex_finalize(NQMutex* thiz)
{
  mutex_destroy(thiz);
  return 0;
}

static inline int NQMutex_lock(NQMutex* thiz)
{
  mutex_lock(thiz);
  return 0;
}

static inline bool NQMutex_trylock(NQMutex* mutex)
{
  return mutex_trylock(mutex) ? true : false;
}

static inline int NQMutex_unlock(NQMutex* thiz)
{
  mutex_unlock(thiz);
  return 0;
}

#endif

#endif /* _LIBNETQ_SYNC_MUTEXKERNEL_H */
