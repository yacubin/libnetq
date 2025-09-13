/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_MUTEX_H
#define _LIBNETQ_MUTEX_H

#include <libnetq/Basic.h>

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#ifdef SRWLOCK_INIT
typedef SRWLOCK NQMutex;
#define NQ_MUTEX_INIT SRWLOCK_INIT
#define NQ_MUTEX_DEFINE(mutexname) NQMutex mutexname = NQ_MUTEX_INIT
#define HAVE_SRWLOCK 1
#else
typedef struct NQMutex {
  CRITICAL_SECTION internalMutex;
  size_t recursionCount;
} NQMutex;
#endif
#endif

#ifdef NQ_OS_UNIX
#include <pthread.h>
typedef pthread_mutex_t NQMutex;
#define NQ_MUTEX_INIT PTHREAD_MUTEX_INITIALIZER
#define NQ_MUTEX_DEFINE(mutexname) NQMutex mutexname = NQ_MUTEX_INIT
#endif

#ifdef NQ_SYS_LINUX
#include <linux/mutex.h>
typedef struct mutex NQMutex;
#define NQ_MUTEX_DEFINE(mutexname) DEFINE_MUTEX(mutexname)
#endif

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT void NQMutex_init(NQMutex* mutex);
NQ_EXPORT void NQMutex_destroy(NQMutex* mutex);
NQ_EXPORT void NQMutex_lock(NQMutex* mutex);
NQ_EXPORT bool NQMutex_trylock(NQMutex* mutex);
NQ_EXPORT void NQMutex_unlock(NQMutex* mutex);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_MUTEX_H */
