/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_PLATFORMLOOPER_H
#define _LIBNETQ_PLATFORMLOOPER_H

#include <libnetq/Looper.h>

#if defined(NQ_OS_WINDOWS)
#include <windows.h>
#elif defined(NQ_OS_DARWIN)
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQPlatformLooper NQPlatformLooper;

struct NQPlatformLooper {
#if defined(NQ_OS_WINDOWS)
  HANDLE eventHandle[1];
  DWORD eventTotal;
  NQTID tid;
#elif defined(NQ_OS_DARWIN)
  CFRunLoopRef runLoop;
  CFRunLoopRunResult lastResult;
#endif
};

NQ_EXPORT void NQPlatformLooper_init(NQPlatformLooper*);
NQ_EXPORT void NQPlatformLooper_finilize(NQPlatformLooper*);
NQ_EXPORT void NQPlatformLooper_wake(NQPlatformLooper*);
NQ_EXPORT void NQPlatformLooper_stop(NQPlatformLooper*);
NQ_EXPORT int NQPlatformLooper_poll(NQPlatformLooper*, int64_t timeout);
NQ_EXPORT bool NQPlatformLooper_getMessage(NQPlatformLooper*, NQMessage* message);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_PLATFORMLOOPER_H */
