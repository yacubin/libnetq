/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_MUTEX_H
#define _LIBNETQ_MUTEX_H

#include <libnetq/Basic.h>

#if defined(NQ_OS_WINDOWS)
# include <libnetq/sync/win32/Mutex.h>
#elif defined(NQ_OS_UNIX)
# include <libnetq/sync/posix/Mutex.h>
#elif defined(NQ_OS_KERNEL)
# include <libnetq/sync/kernel/Mutex.h>
#else
# warning There is no implementation for the Mutex
# include <libnetq/sync/stub/Mutex.h>
#endif

#endif /* _LIBNETQ_MUTEX_H */
