/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_WIN32_COND_H
#define _LIBNETQ_SYNC_WIN32_COND_H

#include <libnetq/Mutex.h>

#ifdef NQ_OS_WINDOWS

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SRWLOCK_INIT
typedef CONDITION_VARIABLE NQCond;
#define HAVE_CONDITION_VARIABLE 1
#else
typedef struct NQCond {
  size_t waitersGone;
  size_t waitersBlocked;
  size_t waitersToUnblock;
  HANDLE blockLock;
  HANDLE blockQueue;
  HANDLE unblockLock;
} NQCond;
#endif

NQ_EXPORT void NQCond_init(NQCond* cond);
NQ_EXPORT void NQCond_finalize(NQCond* cond);
NQ_EXPORT void NQCond_wait(NQCond* cond, NQMutex* mutex);
NQ_EXPORT bool NQCond_waitfor(NQCond* cond, NQMutex* mutex, uint32_t msecs);
NQ_EXPORT void NQCond_signal(NQCond* cond);
NQ_EXPORT void NQCond_broadcast(NQCond* cond);

#ifdef __cplusplus
}
#endif

#endif
#endif /* _LIBNETQ_SYNC_WIN32_COND_H */
