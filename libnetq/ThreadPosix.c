/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQThread"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/Thread.h"

#include <libnetq/OS.h>

#ifdef NQ_OS_UNIX

#include <pthread.h>
#include <errno.h>
#ifdef NQ_OS_LINUX
#include <sys/prctl.h>
#endif

#ifndef HAVE_PTHREAD_YIELD
#include <sched.h>
#endif

#include <libnetq/Mutex.h>
#include <libnetq/Malloc.h>
#include <libnetq/CStrBase.h>
#include <libnetq/Sleep.h>
#include <libnetq/Log.h>
#include <libnetq/Assert.h>

#define HAVE_USLEEP 1

struct NQThread {
  uint32_t flags;
  pthread_t handle;
  void* data;
  NQThreadCallback callback;
  NQMutex mutex;
  char name[1];
};

#define NQThread_state(thread) ((thread->flags) & NQ_THREAD_STATE_MASK)

static inline void NQThread_setState(NQThread* thread, enum NQThreadState state)
{
  thread->flags &= NQ_THREAD_STATE_MASK;
  thread->flags |= state;
}

static void NQThread_setName(pthread_t handle, const char* name)
{
#if defined(HAVE_PTHREAD_SETNAME_NP_1ARG)
  pthread_setname_np(name);
#elif defined(HAVE_PTHREAD_SETNAME_NP_2ARG)
  pthread_setname_np(handle, name);
#elif defined(NQ_OS_LINUX)
  prctl(PR_SET_NAME, name);
#else
  NQ_UNUSED_PARAM(name);
#endif
}

static void* NQThreadEntry(void* context)
{
  bool destroy;
  NQThread* thread = (NQThread*)context;

  NQMutex_lock(&thread->mutex);
  if (thread->name[0] != '\0')
    NQThread_setName(thread->handle, thread->name);
  NQMutex_unlock(&thread->mutex);

  thread->callback(thread->data);

  NQMutex_lock(&thread->mutex);
  thread->flags |= NQ_THREAD_EXITED;
  destroy = thread->flags & NQ_THREAD_DESTROY;
  NQMutex_unlock(&thread->mutex);

  if (destroy)
    NQThread_destroy(thread);

  return 0;
}

static void NQThread_destroyImpl(NQThread* thread)
{
  NQMutex_destroy(&thread->mutex);
  NQFree((void*)thread);
}

NQThread* NQThread_create(NQThreadCallback callback, void* data, const char* name)
{
  if (callback == NULL)
    return NULL;

  size_t len = name ? strlen(name) : 0;
  NQThread* thread = (NQThread*)NQZeroMalloc(sizeof(struct NQThread) + len);
  if (thread == NULL)
    return NULL;

  thread->data = data;
  thread->callback = callback;
  NQMutex_init(&thread->mutex);
  NQThread_setState(thread, NQ_THREAD_JOINABLE);

  if (name != NULL && name[0] != '\0')
    memcpy(thread->name, name, len);
  thread->name[len] = '\0';

  pthread_attr_t attr;
  pthread_attr_init(&attr);
#ifdef HAVE_PTHREAD_ATTR_SET_QOS_CLASS_NP
  pthread_attr_set_qos_class_np(&attr, QOS_CLASS_USER_INITIATED, 0);
#endif
  int error = pthread_create(&thread->handle, &attr, NQThreadEntry, thread);
  pthread_attr_destroy(&attr);

  if (error) {
      NQ_LOGE("Failed to create pthread at entry point %p with context %p", (void*)NQThreadEntry, (void*)thread);
      NQThread_destroyImpl(thread);
      return NULL;
  }

  return thread;
}

void NQThread_destroy(NQThread* thread)
{
  NQMutex_lock(&thread->mutex);
  thread->flags |= NQ_THREAD_DESTROY;
  bool exited = thread->flags & NQ_THREAD_EXITED;
  NQMutex_unlock(&thread->mutex);

  if (exited)
    NQThread_destroyImpl(thread);
}

NQTID NQThread_id(NQThread* thread)
{
  NQMutex_lock(&thread->mutex);
  NQTID tid = (NQTID)thread->handle;
  NQMutex_unlock(&thread->mutex);
  return tid;
}

void NQThread_setPriority(NQThread* thread, int delta)
{
#ifdef HAVE_PTHREAD_SETSCHEDPARAM
    int policy;
    struct sched_param param;

    NQMutex_lock(&thread->mutex);
    if (!pthread_getschedparam(thread->handle, &policy, &param)) {
      param.sched_priority += delta;
      pthread_setschedparam(thread->handle, policy, &param);
    }
    NQMutex_unlock(&thread->mutex);
#endif
}

int NQThread_join(NQThread* thread)
{
  NQMutex_lock(&thread->mutex);
  pthread_t handle = thread->handle;
  NQMutex_unlock(&thread->mutex);

  int joinResult = pthread_join(handle, 0);
  if (joinResult == EDEADLK)
      NQ_LOGE("Thread %p was found to be deadlocked trying to quit", (void*)thread);
  else if (joinResult)
      NQ_LOGE("Thread %p was unable to be joined.\n", (void*)thread);

  NQMutex_lock(&thread->mutex);
  NQ_ASSERT(NQThread_state(thread) == NQ_THREAD_JOINABLE);
  if (!(thread->flags & NQ_THREAD_EXITED))
    NQThread_setState(thread, NQ_THREAD_JOINED);
  NQMutex_unlock(&thread->mutex);

  return joinResult;
}

void NQThread_detach(NQThread* thread)
{
  NQMutex_lock(&thread->mutex);

  int detachResult = pthread_detach(thread->handle);
  if (detachResult)
      NQ_LOGE("Thread %p was unable to be detached\n", (void*)thread);

  if (!(thread->flags & NQ_THREAD_EXITED))
    NQThread_setState(thread, NQ_THREAD_DETACHED);
  NQMutex_unlock(&thread->mutex);
}

int NQThread_suspend(NQThread* thread)
{
  NQ_ASSERT_NOT_REACHED();
  return -1;
}

void NQThread_resume(NQThread* thread)
{
  NQ_ASSERT_NOT_REACHED();
}

NQTID NQThreadId(void)
{
  return (NQTID)pthread_self();
}

void NQThreadYield(void)
{
#ifdef HAVE_PTHREAD_YIELD
  pthread_yield();
#else
  sched_yield();
#endif
}

void NQThreadSleep(int32_t ms)
{
  NQSleep(ms);
}

#endif
