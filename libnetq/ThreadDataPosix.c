/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/ThreadData.h"

#include <libnetq/OS.h>

#ifdef NQ_OS_UNIX

#include <limits.h>
#include <pthread.h>

#include <libnetq/Assert.h>

#ifdef PTHREAD_KEYS_MAX
#define NQ_THREADDATA_OFFSET PTHREAD_KEYS_MAX
#else
#define NQ_THREADDATA_OFFSET 1
#endif

# define TO_THREAD_DATA(key) ((NQThreadData*)(((uintptr_t)(key)) + NQ_THREADDATA_OFFSET))
# define TO_PTHREAD_KEY(data) ((pthread_key_t)(((uintptr_t)(data)) - NQ_THREADDATA_OFFSET))

NQThreadData* NQThreadData_create(NQThreadDataCallback callback)
{
  pthread_key_t key;
  if (!pthread_key_create(&key, callback))
    return TO_THREAD_DATA(key);
  return NULL;
}

void NQThreadData_destroy(NQThreadData* data)
{
  pthread_key_t key = TO_PTHREAD_KEY(data);
  pthread_key_delete(key);
}

void* NQThreadData_get(NQThreadData* data)
{
  pthread_key_t key = TO_PTHREAD_KEY(data);
  return pthread_getspecific(key);
}

bool NQThreadData_set(NQThreadData* data, const void* ptr)
{
  pthread_key_t key = TO_PTHREAD_KEY(data);
  return pthread_setspecific(key, ptr) ? false : true;
}

#endif
