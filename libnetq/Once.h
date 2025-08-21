/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ONCE_H
#define _LIBNETQ_ONCE_H

#include <libnetq/Basic.h>

#ifdef NQ_OS_WIN
#include <windows.h>
typedef INIT_ONCE NQOnce;
#define NQ_ONCE_INIT INIT_ONCE_STATIC_INIT
#endif

#ifdef NQ_OS_UNIX
#include <pthread.h>
typedef pthread_once_t NQOnce;
#define NQ_ONCE_INIT PTHREAD_ONCE_INIT
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*NQOnceCallback) (void);

NQ_EXPORT int NQOnce_call(NQOnce* once, NQOnceCallback callback);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ONCE_H */
