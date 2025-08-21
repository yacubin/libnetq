/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Mutex.h"

#ifdef NQ_OS_WIN

#include <libnetq/Compiler.h>
#include <libnetq/Assert.h>

#ifdef HAVE_SRWLOCK
// http://locklessinc.com/articles/pthreads_on_windows/

void NQMutex_init(NQMutex* mutex)
{
  InitializeSRWLock(mutex);
}

void NQMutex_destroy(NQMutex* mutex)
{
  NQ_UNUSED_PARAM(mutex);
}

void NQMutex_lock(NQMutex* mutex)
{
  AcquireSRWLockExclusive(mutex);
}

bool NQMutex_trylock(NQMutex* mutex)
{
  return TryAcquireSRWLockExclusive(mutex);
}

void NQMutex_unlock(NQMutex* mutex)
{
  ReleaseSRWLockExclusive(mutex);
}
#else
void NQMutex_init(NQMutex* mutex)
{
  mutex->recursionCount = 0;
  InitializeCriticalSection(&mutex->internalMutex);
}

void NQMutex_destroy(NQMutex* mutex)
{
  DeleteCriticalSection(&mutex->internalMutex);
}

void NQMutex_lock(NQMutex* mutex)
{
  EnterCriticalSection(&mutex->internalMutex);
  ++mutex->recursionCount;
}

#pragma warning(suppress: 26115)
bool NQMutex_trylock(NQMutex* mutex)
{
  DWORD result = TryEnterCriticalSection(&mutex->internalMutex);

  if (result != 0) {
    if (mutex->recursionCount > 0) {
      LeaveCriticalSection(&mutex->internalMutex);
      return false;
    }

    ++mutex->recursionCount;
    return true;
  }

  return false;
}

void NQMutex_unlock(NQMutex* mutex)
{
  NQ_ASSERT(mutex->recursionCount);
  --mutex->recursionCount;
  LeaveCriticalSection(&mutex->internalMutex);
}

#endif

#endif /* NQ_OS_WIN */
