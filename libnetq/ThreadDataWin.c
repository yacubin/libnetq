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

#ifdef NQ_OS_WINDOWS

#include <windows.h>

#include <libnetq/Malloc.h>
#include <libnetq/Assert.h>

#define NQ_THREADDATA_ALLOW_FLS 1

#if defined(FLS_OUT_OF_INDEXES) && NQ_THREADDATA_ALLOW_FLS
struct NQThreadData {
  DWORD key;
  NQThreadDataCallback callback;
};
struct NQThreadSpecific {
  void* ptr;
  NQThreadDataCallback callback;
};
static VOID NTAPI pflsCallback(PVOID lpFlsData)
{
  struct NQThreadSpecific* specific = (struct NQThreadSpecific*)lpFlsData;
  if (specific == NULL)
    return;

  if (specific->callback != NULL)
    specific->callback(specific->ptr);

  NQFree((void*)specific);
}
#endif

NQThreadData* NQThreadData_create(NQThreadDataCallback callback)
{
#if defined(FLS_OUT_OF_INDEXES) && NQ_THREADDATA_ALLOW_FLS
  DWORD key = FlsAlloc(pflsCallback);
  if (key != FLS_OUT_OF_INDEXES) {
    NQThreadData* data = (NQThreadData*)NQMalloc(sizeof(NQThreadData));
    if (data != NULL) {
      data->key = key;
      data->callback = callback;
      return data;
    }
    FlsFree(key);
  }
  return NULL;

#elif defined(TLS_OUT_OF_INDEXES)
  if (callback == NULL) {
    DWORD key = TlsAlloc();
    if (key != TLS_OUT_OF_INDEXES) {
      key -= TLS_OUT_OF_INDEXES;
      return (NQThreadData*)((uintptr_t)key);
    }
  }
  return NULL;

#else
  return NULL;

#endif
}

void NQThreadData_destroy(NQThreadData* data)
{
  NQ_ASSERT(data != NULL);

#if defined(FLS_OUT_OF_INDEXES) && NQ_THREADDATA_ALLOW_FLS
  FlsFree(data->key);
  NQFree((void*)data);

#elif defined(TLS_OUT_OF_INDEXES)
  DWORD key = (DWORD)((uintptr_t)data);
  key += TLS_OUT_OF_INDEXES;
  TlsFree(key);

#endif
}


void* NQThreadData_get(NQThreadData* data)
{
  NQ_ASSERT(data != NULL);

#if defined(FLS_OUT_OF_INDEXES) && NQ_THREADDATA_ALLOW_FLS
  struct NQThreadSpecific* specific = (struct NQThreadSpecific*)FlsGetValue(data->key);
  if (specific != NULL)
    return NULL;
  return specific->ptr;

#elif defined(TLS_OUT_OF_INDEXES)
  DWORD key = (DWORD)((uintptr_t)data);
  key += TLS_OUT_OF_INDEXES;
  return TlsGetValue(key);

#else
  return NULL;

#endif
}

bool NQThreadData_set(NQThreadData* data, const void* ptr)
{
  NQ_ASSERT(data != NULL);

#if defined(FLS_OUT_OF_INDEXES) && NQ_THREADDATA_ALLOW_FLS
  struct NQThreadSpecific* specific = (struct NQThreadSpecific*)FlsGetValue(data->key);

  if (ptr == NULL) {
    if (specific != NULL) {
      if (!FlsSetValue(data->key, NULL))
        return false;
      NQFree((void*)specific);
    }
    return true;
  }

  if (specific != NULL) {
    specific->ptr = (void*)ptr;
    return true;
  }

  specific = (struct NQThreadSpecific*)NQMalloc(sizeof(struct NQThreadSpecific));
  if (specific != NULL) {
    specific->ptr = (void*)ptr;
    specific->callback = data->callback;
    if (FlsSetValue(data->key, (PVOID)specific))
      return true;
    NQFree((void*)specific);
  }

  return false;

#elif defined(TLS_OUT_OF_INDEXES)
  DWORD key = (DWORD)((uintptr_t)data);
  key += TLS_OUT_OF_INDEXES;
  if (TlsSetValue(key, (PVOID)ptr))
    return true;
  return false;

#else
  return false;

#endif

}

#endif
