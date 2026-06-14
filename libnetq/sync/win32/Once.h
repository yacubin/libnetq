/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_WIN32_ONCE_H
#define _LIBNETQ_SYNC_WIN32_ONCE_H

#include <libnetq/Basic.h>
#include <libnetq/sync/OnceCallback.h>

#ifdef NQ_OS_WINDOWS

#include <windows.h>

typedef INIT_ONCE NQOnce;

#define NQ_ONCE_INIT INIT_ONCE_STATIC_INIT

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT int NQOnce_call(NQOnce* once, NQOnceCallback callback);

#ifdef __cplusplus
}
#endif

#endif /* NQ_OS_WINDOWS */
#endif /* _LIBNETQ_SYNC_WIN32_ONCE_H */
