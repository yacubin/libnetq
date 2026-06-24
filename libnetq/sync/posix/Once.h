/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_POSIX_ONCE_H
#define _LIBNETQ_SYNC_POSIX_ONCE_H

#include <libnetq/Basic.h>
#include <libnetq/sync/OnceCallback.h>

#ifdef NQ_OS_UNIX

#include <pthread.h>

typedef pthread_once_t NQOnce;

#define NQ_ONCE_INIT PTHREAD_ONCE_INIT

static inline int NQOnce_call(NQOnce* once, NQOnceCallback callback)
{
  return pthread_once(once, callback);
}

#endif /* NQ_OS_UNIX */
#endif /* _LIBNETQ_SYNC_POSIX_ONCE_H */
