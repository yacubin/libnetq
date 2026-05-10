/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_WIN32_MUTEX_H
#define _LIBNETQ_SYNC_WIN32_MUTEX_H

#include <libnetq/Basic.h>

#ifdef NQ_OS_WINDOWS
#include <windows.h>

#ifdef SRWLOCK_INIT
#define HAVE_SRWLOCK 1
// http://locklessinc.com/articles/pthreads_on_windows

typedef SRWLOCK NQMutex;
#define NQ_MUTEX_INIT SRWLOCK_INIT
#define NQ_MUTEX_DEFINE(mutexname) NQMutex mutexname = NQ_MUTEX_INIT

static inline int NQMutex_init(NQMutex* thiz)
{
  InitializeSRWLock(thiz);
  return 0;
}

static inline int NQMutex_finalize(NQMutex* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return 0;
}

static inline int NQMutex_lock(NQMutex* thiz)
{
  AcquireSRWLockExclusive(thiz);
  return 0;
}

static inline bool NQMutex_trylock(NQMutex* thiz)
{
  return TryAcquireSRWLockExclusive(thiz) ? true : false;
}

static inline int NQMutex_unlock(NQMutex* thiz)
{
  ReleaseSRWLockExclusive(thiz);
  return 0;
}

#else

typedef struct NQMutex NQMutex;
struct NQMutex {
  CRITICAL_SECTION internalMutex;
  unsigned recursionCount;
};

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT int NQMutex_init(NQMutex*);
NQ_EXPORT int NQMutex_finalize(NQMutex*);
NQ_EXPORT int NQMutex_lock(NQMutex*);
NQ_EXPORT bool NQMutex_trylock(NQMutex*);
NQ_EXPORT int NQMutex_unlock(NQMutex*);

#ifdef __cplusplus
}
#endif

#endif

#endif

#endif /* _LIBNETQ_SYNC_WIN32_MUTEX_H */
