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

#ifdef NQ_OS_UNIX

#include <errno.h>
#include <libnetq/Limits.h>
#include <libnetq/Assert.h>

void NQCond_init(NQCond* cond)
{
  pthread_condattr_t attr;
  pthread_condattr_init(&attr);
#if HAVE_PTHREAD_CONDATTR_SETCLOCK
  pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
#endif
  pthread_cond_init(cond, &attr);
  pthread_condattr_destroy(&attr);
}

void NQCond_destroy(NQCond* cond)
{
  pthread_cond_destroy(cond);
}

void NQCond_wait(NQCond* cond, NQMutex* mutex)
{
  int result = pthread_cond_wait(cond, mutex);
  NQ_ASSERT_UNUSED(result, !result);
}

bool NQCond_waitfor(NQCond* cond, NQMutex* mutex, uint32_t ms)
{
  //if (abstime < npf_time_now())
  //  return false;

  if (ms == NQ_UINT32_MAX) {
    NQCond_wait(cond, mutex);
    return true;
  }

  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  int timeSeconds = (int)(ms / 1000);
  int timeNanoseconds = (int)((ms - timeSeconds) * 1000000);

  ts.tv_sec += timeSeconds;
  ts.tv_nsec += timeNanoseconds;

  return pthread_cond_timedwait(cond, mutex, &ts) == 0;
}

void NQCond_signal(NQCond* cond)
{
  int result = pthread_cond_signal(cond);
  NQ_ASSERT_UNUSED(result, !result);
}

void NQCond_broadcast(NQCond* cond)
{
  int result = pthread_cond_broadcast(cond);
  NQ_ASSERT_UNUSED(result, !result);
}

#endif /* NQ_OS_UNIX */
