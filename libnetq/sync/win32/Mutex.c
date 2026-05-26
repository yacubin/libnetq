/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/sync/win32/Mutex.h"

#ifdef NQ_OS_WINDOWS
#ifndef HAVE_SRWLOCK

#include <libnetq/Assert.h>

int NQMutex_init(NQMutex* thiz)
{
  thiz->recursionCount = 0;
  InitializeCriticalSection(&thiz->internalMutex);
  return 0;
}

int NQMutex_finalize(NQMutex* thiz)
{
  DeleteCriticalSection(&thiz->internalMutex);
  return 0;
}

int NQMutex_lock(NQMutex* thiz)
{
  EnterCriticalSection(&thiz->internalMutex);
  ++thiz->recursionCount;
  return 0;
}

#pragma warning(suppress: 26115)
bool NQMutex_trylock(NQMutex* thiz)
{
  DWORD result = TryEnterCriticalSection(&thiz->internalMutex);

  if (result != 0) {
    if (thiz->recursionCount > 0) {
      LeaveCriticalSection(&thiz->internalMutex);
      return false;
    }

    ++thiz->recursionCount;
    return true;
  }

  return false;
}

int NQMutex_unlock(NQMutex* thiz)
{
  NQ_ASSERT(thiz->recursionCount);
  --thiz->recursionCount;
  LeaveCriticalSection(&thiz->internalMutex);
  return 0;
}

#endif
#endif /* NQ_OS_WINDOWS */
