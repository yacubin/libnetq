/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Cond.h"

#ifdef NQ_OS_WINDOWS
#include <libnetq/Assert.h>

#ifdef HAVE_CONDITION_VARIABLE
void NQCond_init(NQCond* cond)
{
  InitializeConditionVariable(cond);
}

void NQCond_destroy(NQCond* cond)
{
  NQ_ASSERT_UNUSED(cond, cond);
}

void NQCond_wait(NQCond* cond, NQMutex* mutex)
{
  SleepConditionVariableSRW(cond, mutex, INFINITE, 0);
}

bool NQCond_waitfor(NQCond* cond, NQMutex* mutex, uint32_t ms)
{
  if (SleepConditionVariableSRW(cond, mutex, ms, 0))
    return true;

  NQ_ASSERT(GetLastError() == ERROR_TIMEOUT);
  return false;
}

void NQCond_signal(NQCond* cond)
{
  WakeConditionVariable(cond);
}

void NQCond_broadcast(NQCond* cond)
{
  WakeAllConditionVariable(cond);
}

#else

static const long kMaxSemaphoreCount = (long)(~0UL >> 1);

void NQCond_init(NQCond* cond)
{
  cond->waitersGone = 0;
  cond->waitersBlocked = 0;
  cond->waitersToUnblock = 0;
  cond->blockLock = CreateSemaphore(0, 1, 1, 0);
  cond->blockQueue = CreateSemaphore(0, 0, kMaxSemaphoreCount, 0);
  cond->unblockLock = CreateMutex(0, 0, 0);

  if (!cond->blockLock || !cond->blockQueue || !cond->unblockLock) {
    if (cond->blockLock)
      CloseHandle(cond->blockLock);
    if (cond->blockQueue)
      CloseHandle(cond->blockQueue);
    if (cond->unblockLock)
      CloseHandle(cond->unblockLock);
  }
}

void NQCond_destroy(NQCond* cond)
{
  CloseHandle(cond->blockLock);
  CloseHandle(cond->blockQueue);
  CloseHandle(cond->unblockLock);
}

static inline bool NQCond_waitImpl(NQCond* cond, NQMutex* mutex, DWORD durationMilliseconds)
{
  DWORD res = WaitForSingleObject(cond->blockLock, INFINITE);
  NQ_ASSERT_UNUSED(res, res == WAIT_OBJECT_0);
  ++cond->waitersBlocked;
  res = ReleaseSemaphore(cond->blockLock, 1, 0);
  NQ_ASSERT_UNUSED(res, res);

  --mutex->recursionCount;
  LeaveCriticalSection(&mutex->internalMutex);

  // Main wait - use timeout.
  bool timedOut = (WaitForSingleObject(cond->blockQueue, durationMilliseconds) == WAIT_TIMEOUT);

  res = WaitForSingleObject(cond->unblockLock, INFINITE);
  NQ_ASSERT_UNUSED(res, res == WAIT_OBJECT_0);

  size_t signalsLeft = cond->waitersToUnblock;

  if (cond->waitersToUnblock)
    --cond->waitersToUnblock;
  else if (++cond->waitersGone == (INT_MAX / 2)) {
    res = WaitForSingleObject(cond->blockLock, INFINITE);
    NQ_ASSERT_UNUSED(res, res == WAIT_OBJECT_0);
    cond->waitersBlocked -= cond->waitersGone;
    res = ReleaseSemaphore(cond->blockLock, 1, 0);
    NQ_ASSERT_UNUSED(res, res);
    cond->waitersGone = 0;
  }

  res = ReleaseMutex(cond->unblockLock);
  NQ_ASSERT_UNUSED(res, res);

  if (signalsLeft == 1) {
    res = ReleaseSemaphore(cond->blockLock, 1, 0); // Open the gate.
    NQ_ASSERT_UNUSED(res, res);
  }

  EnterCriticalSection(&mutex->internalMutex);
  ++mutex->recursionCount;

  return !timedOut;
}

void NQCond_wait(NQCond* cond, NQMutex* mutex)
{
  NQCond_waitImpl(cond, mutex, INFINITE);
}

bool NQCond_waitfor(NQCond* cond, NQMutex* mutex, uint32_t ms)
{
  return NQCond_waitImpl(cond, mutex, (DWORD)ms);
}

static inline void NQCond_signalImpl(NQCond* cond, bool unblockAll)
{
  size_t signalsToIssue = 0;

  DWORD res = WaitForSingleObject(cond->unblockLock, INFINITE);
  NQ_ASSERT_UNUSED(res, res == WAIT_OBJECT_0);

  if (cond->waitersToUnblock) {
    if (!cond->waitersBlocked) {
      res = ReleaseMutex(cond->unblockLock);
      NQ_ASSERT_UNUSED(res, res);
      return;
    }

    if (unblockAll) {
      signalsToIssue = cond->waitersBlocked;
      cond->waitersToUnblock += cond->waitersBlocked;
      cond->waitersBlocked = 0;
    }
    else {
      signalsToIssue = 1;
      ++cond->waitersToUnblock;
      --cond->waitersBlocked;
    }
  }
  else if (cond->waitersBlocked > cond->waitersGone) {
    res = WaitForSingleObject(cond->blockLock, INFINITE);
    NQ_ASSERT_UNUSED(res, res == WAIT_OBJECT_0);
    if (cond->waitersGone != 0) {
      cond->waitersBlocked -= cond->waitersGone;
      cond->waitersGone = 0;
    }
    if (unblockAll) {
      signalsToIssue = cond->waitersBlocked;
      cond->waitersToUnblock = cond->waitersBlocked;
      cond->waitersBlocked = 0;
    }
    else {
      signalsToIssue = 1;
      cond->waitersToUnblock = 1;
      --cond->waitersBlocked;
    }
  }
  else {
    res = ReleaseMutex(cond->unblockLock);
    NQ_ASSERT_UNUSED(res, res);
    return;
  }

  res = ReleaseMutex(cond->unblockLock);
  NQ_ASSERT_UNUSED(res, res);

  if (signalsToIssue) {
    res = ReleaseSemaphore(cond->blockQueue, signalsToIssue, 0);
    NQ_ASSERT_UNUSED(res, res);
  }
}

void NQCond_signal(NQCond* cond)
{
  NQCond_signalImpl(cond, false);
}

void NQCond_broadcast(NQCond* cond)
{
  NQCond_signalImpl(cond, true);
}

#endif

#endif /* NQ_OS_WIN */
