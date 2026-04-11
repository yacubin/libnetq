/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQThread"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/Thread.h"

#ifdef NQ_SYS_LINUX

#include <libnetq/Malloc.h>
#include <libnetq/String.h>
#include <libnetq/Math.h>

#include <linux/kthread.h>
#include <linux/delay.h>

struct NQThread {
  uint32_t flags;
  struct task_struct* handle;
  void* data;
  NQThreadCallback callback;
  char name[1];
};

static int threadWork(void* data)
{
  NQThread* thiz = (NQThread*)data;
  
  
  thiz->callback(thiz->data);

  return 0;
}

NQThread* NQThread_create(NQThreadCallback callback, void* data, const char* name)
{
  size_t len = name ? strlen(name) : 0;
  NQThread* thiz = (NQThread*)NQZeroMalloc(sizeof(struct NQThread) + len);
  if (thiz == NULL)
    return NULL;

  thiz->data = data;
  thiz->callback = callback;

  if (name != NULL && name[0] != '\0')
    memcpy(thiz->name, name, len);
  thiz->name[len] = '\0';

  thiz->handle = kthread_run(threadWork, thiz, thiz->name);
  if (IS_ERR(thiz->handle)) {
    NQFree(thiz);
    return NULL;
  }
  
  return thiz;
}

void NQThread_destroy(NQThread* thiz)
{
  if (thiz->handle) {
    kthread_stop(thiz->handle);
  }
  NQFree(thiz);
}

NQTID NQThread_id(NQThread* thiz)
{
  return thiz->handle->pid;
}

void NQThread_setPriority(NQThread* thiz, int delta)
{
  int newNice = task_nice(thiz->handle) + delta;
  set_user_nice(thiz->handle, NQGetClamp(newNice, MIN_NICE, MAX_NICE));
}

int NQThread_join(NQThread* thiz)
{
  return kthread_stop(thiz->handle);
}

void NQThread_detach(NQThread* thiz)
{
  thiz->handle = NULL;
}

int NQThread_suspend(NQThread* thiz)
{
  return kthread_park(thiz->handle);
}

void NQThread_resume(NQThread* thiz)
{
  kthread_unpark(thiz->handle);
}

NQTID NQThreadId(void)
{
  return current->pid;
}

void NQThreadYield(void)
{
  cond_resched();
}

void NQThreadSleep(int32_t ms)
{
  msleep(ms);
}

#endif
