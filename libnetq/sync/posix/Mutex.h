/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_POSIX_MUTEX_H
#define _LIBNETQ_SYNC_POSIX_MUTEX_H

#include <libnetq/Basic.h>

#ifdef NQ_OS_UNIX

#include <pthread.h>

typedef pthread_mutex_t NQMutex;
#define NQ_MUTEX_INIT PTHREAD_MUTEX_INITIALIZER
#define NQ_MUTEX_DEFINE(mutexname) NQMutex mutexname = NQ_MUTEX_INIT

static inline int NQMutex_init(NQMutex* thiz)
{
  return pthread_mutex_init(thiz, NULL);
}

static inline int NQMutex_finalize(NQMutex* thiz)
{
  return pthread_mutex_destroy(thiz);
}

static inline int NQMutex_lock(NQMutex* thiz)
{
  return pthread_mutex_lock(thiz);
}

static inline bool NQMutex_trylock(NQMutex* thiz)
{
  return pthread_mutex_trylock(thiz) == 0 ? true : false;
}

static inline int NQMutex_unlock(NQMutex* thiz)
{
  return pthread_mutex_unlock(thiz);
}

#endif

#endif /* _LIBNETQ_SYNC_POSIX_MUTEX_H */
