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

#if (defined(NQ_COMPILER_MINGW) && _WIN32_WINNT >= 0x0600) || \
    (defined(NQ_COMPILER_MSVC) && defined(SRWLOCK_INIT))
#define NQ_HAVE_ARCH_SRWLOCK 1
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

#include <libnetq/Assert.h>

typedef struct NQMutex NQMutex;
struct NQMutex {
  volatile LONG init;
  unsigned recursionCount;
  CRITICAL_SECTION cs;
};

#define NQ_MUTEX_INIT { 0 }
#define NQ_MUTEX_DEFINE(mutexname) NQMutex mutexname = NQ_MUTEX_INIT

static inline int NQMutex_init(NQMutex* thiz)
{
  thiz->recursionCount = 0;
  InitializeCriticalSection(&thiz->cs);
  InterlockedExchange(&thiz->init, 2); // should be last
  return 0;
}

static inline int NQMutex_finalize(NQMutex* thiz)
{
  DeleteCriticalSection(&thiz->cs);
  return 0;
}

static inline void NQMutex_initOnce(NQMutex* thiz)
{
  if (thiz->init != 2) {
    if (InterlockedCompareExchange(&thiz->init, 1, 0) == 0) {
      NQMutex_init(thiz);
      return;
    }
    while (thiz->init != 2) {
        YieldProcessor();
    }
  }
}

static inline int NQMutex_lock(NQMutex* thiz)
{
  NQMutex_initOnce(thiz);
  EnterCriticalSection(&thiz->cs);
  ++thiz->recursionCount;
  return 0;
}

#pragma warning(suppress: 26115)
static inline bool NQMutex_trylock(NQMutex* thiz)
{
  NQMutex_initOnce(thiz);
  if (TryEnterCriticalSection(&thiz->cs)) {
    if (thiz->recursionCount == 0) {
      ++thiz->recursionCount;
      return true;
    }
    LeaveCriticalSection(&thiz->cs);
  }
  return false;
}

static inline int NQMutex_unlock(NQMutex* thiz)
{
  NQ_ASSERT(thiz->recursionCount);
  --thiz->recursionCount;
  LeaveCriticalSection(&thiz->cs);
  return 0;
}

#endif

#endif

#endif /* _LIBNETQ_SYNC_WIN32_MUTEX_H */
