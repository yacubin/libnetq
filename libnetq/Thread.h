/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_THREAD_H
#define _LIBNETQ_THREAD_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif
  
typedef uintptr_t NQTID;
typedef struct NQThread NQThread;
typedef void (*NQThreadCallback)(void*);

enum NQThreadState {
  NQ_THREAD_JOINABLE = 0,
  NQ_THREAD_JOINED = 1,
  NQ_THREAD_DETACHED = 2,
};

#define NQ_THREAD_STATE_MASK 3

enum NQThreadFlag {
  NQ_THREAD_EXITED = (1 << 2),
  NQ_THREAD_DESTROY = (1 << 3),
};

NQ_EXPORT NQThread* NQThread_create(NQThreadCallback callback, void* data, const char* name);
NQ_EXPORT void NQThread_destroy(NQThread* thread);

NQ_EXPORT NQTID NQThread_id(NQThread* thread);
NQ_EXPORT void NQThread_setPriority(NQThread* thread, int delta);
NQ_EXPORT int NQThread_join(NQThread* thread);
NQ_EXPORT void NQThread_detach(NQThread* thread);
NQ_EXPORT int NQThread_suspend(NQThread* thread);
NQ_EXPORT void NQThread_resume(NQThread* thread);

NQ_EXPORT NQTID NQThreadId(void);
NQ_EXPORT void NQThreadYield(void);
NQ_EXPORT void NQThreadSleep(int32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_THREAD_H */
