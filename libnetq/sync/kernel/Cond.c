/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/sync/kernel/Cond.h"

#ifdef NQ_OS_KERNEL

#include <linux/jiffies.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

#include <libnetq/ErrorCode.h>

void NQCond_init(NQCond* cond)
{
  init_waitqueue_head(&cond->waitQueue);
}

void NQCond_finalize(NQCond* cond)
{
}

void NQCond_wait(NQCond* cond, NQMutex* mutex)
{
  DEFINE_WAIT(wait);
  prepare_to_wait(&cond->waitQueue, &wait, TASK_INTERRUPTIBLE);
  NQMutex_unlock(mutex);
  schedule();
  NQMutex_lock(mutex);
  finish_wait(&cond->waitQueue, &wait);
}

bool NQCond_waitfor(NQCond* cond, NQMutex* mutex, uint32_t msecs)
{
  long timeout = msecs_to_jiffies(msecs);
  int ret = 0;

  DEFINE_WAIT(wait);

  prepare_to_wait(&cond->waitQueue, &wait, TASK_INTERRUPTIBLE);
  NQMutex_unlock(mutex);
  timeout = schedule_timeout(timeout);
  NQMutex_lock(mutex);
  finish_wait(&cond->waitQueue, &wait);

  if (timeout == 0)
    ret = -NQ_ETIMEDOUT;
  else if (signal_pending(current))
    ret = -NQ_ERESTARTSYS;

  return ret;
}

void NQCond_signal(NQCond* cond)
{
  wake_up_interruptible(&cond->waitQueue);
}

void NQCond_broadcast(NQCond* cond)
{
  wake_up_interruptible_all(&cond->waitQueue);
}

#endif /* NQ_OS_UNIX */
