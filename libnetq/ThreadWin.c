/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQThread"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/Thread.h"

#ifdef NQ_OS_WINDOWS

#include <windows.h>

#include <libnetq/ObjectClass.h>
#include <libnetq/Mutex.h>
#include <libnetq/Malloc.h>
#include <libnetq/Compiler.h>
#include <libnetq/CStrBase.h>
#include <libnetq/Log.h>
#include <libnetq/Assert.h>

extern const NQObjectClass __NQThreadClass;

struct NQThread {
  const NQObjectClass* class;
  DWORD identifier;
  uint32_t flags;
  HANDLE handle;
  void* data;
  NQThreadCallback callback;
  NQMutex mutex;
  char name[1];
};

static const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push, 8)
typedef struct tagTHREADNAME_INFO {
  DWORD dwType;     // must be 0x1000
  LPCSTR szName;    // pointer to name (in user addr space)
  DWORD dwThreadID; // thread ID (-1=caller thread)
  DWORD dwFlags;    // reserved for future use, must be zero
} THREADNAME_INFO;
#pragma pack(pop)

static void NQThread_setName(const char* name)
{
#if NQ_COMPILER_MINGW
  NQ_UNUSED_PARAM(name);
#else
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = name;
  info.dwThreadID = GetCurrentThreadId();
  info.dwFlags = 0;

  __try {
    RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)(&info));
  }
  __except (EXCEPTION_CONTINUE_EXECUTION) {
  }
#endif
}

#define NQThread_state(thread) ((thread->flags) & NQ_THREAD_STATE_MASK)

static inline void NQThread_setState(NQThread* thread, enum NQThreadState state)
{
  thread->flags &= NQ_THREAD_STATE_MASK;
  thread->flags |= state;
}

static void NQThread_destroyImpl(NQThread* thread)
{
  NQMutex_destroy(&thread->mutex);

  if (thread->handle)
    CloseHandle(thread->handle);
  
  NQFree((void*)thread);
}

static DWORD WINAPI NQThreadEntry(LPVOID lpParam)
{
  bool destroy;
  NQThread* thread = (NQThread*)lpParam;

  NQMutex_lock(&thread->mutex);
  if (thread->name[0] != '\0')
    NQThread_setName(thread->name);
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

NQThread* NQThread_create(NQThreadCallback callback, void* data, const char* name)
{
  if (callback == NULL)
    return NULL;
  
  size_t len = name ? strlen(name) : 0;
  NQThread* thread = (NQThread*)NQZeroMalloc(sizeof(struct NQThread) + len);
  if (thread == NULL)
    return NULL;

  thread->class = &__NQThreadClass;
  thread->data = data;
  thread->callback = callback;
  NQMutex_init(&thread->mutex);
  NQThread_setState(thread, NQ_THREAD_JOINABLE);

  if (name != NULL && name[0] != '\0')
    memcpy(thread->name, name, len);
  thread->name[len] = '\0';

  thread->handle = CreateThread(NULL, 0, NQThreadEntry, thread, 0, &thread->identifier);

  if (thread->handle != NULL)
    return thread;

  NQThread_destroyImpl(thread);
  return NULL;
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
  NQTID tid = thread->identifier;
  NQMutex_unlock(&thread->mutex);
  return tid;
}

void NQThread_setPriority(NQThread* thread, int delta)
{
  NQMutex_lock(&thread->mutex);
  SetThreadPriority(thread->handle, THREAD_PRIORITY_NORMAL + delta);
  NQMutex_unlock(&thread->mutex);
}

int NQThread_join(NQThread* thread)
{
  NQMutex_lock(&thread->mutex);
  HANDLE handle = thread->handle;
  NQMutex_unlock(&thread->mutex);

  DWORD joinResult = WaitForSingleObject(handle, INFINITE);
  if (joinResult == WAIT_FAILED)
    NQ_LOGE("Thread %p was found to be deadlocked trying to quit", thread);
  
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
  if (!(thread->flags & NQ_THREAD_EXITED))
    NQThread_setState(thread, NQ_THREAD_DETACHED);
  NQMutex_unlock(&thread->mutex);
}

int NQThread_suspend(NQThread* thread)
{
  NQMutex_lock(&thread->mutex);
  DWORD result = SuspendThread(thread->handle);
  NQMutex_unlock(&thread->mutex);

  return (int)result;
}

void NQThread_resume(NQThread* thread)
{
  NQMutex_lock(&thread->mutex);
  ResumeThread(thread->handle);
  NQMutex_unlock(&thread->mutex);
}

NQTID NQThreadId()
{
  return (NQTID)GetCurrentThreadId();
}

void NQThreadYield()
{
  // Sleep(0);
  // YieldProcessor();
  SwitchToThread();
}

void NQThreadSleep(int32_t ms)
{
  NQ_ASSERT(ms >= 0);
  Sleep((DWORD)ms);
}

const NQObjectClass __NQThreadClass = {
  NQThreadObjectType,
  NQ_CLASS_NAME,
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQThread_destroy,
};

#endif
