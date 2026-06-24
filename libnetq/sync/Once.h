/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_ONCE_H
#define _LIBNETQ_SYNC_ONCE_H

#include <libnetq/Basic.h>

#if defined(NQ_OS_KERNEL)
# include <libnetq/sync/kernel/Once.h>
#elif defined(NQ_OS_WINDOWS)
# include <libnetq/sync/win32/Once.h>
#elif defined(NQ_OS_UNIX)
# include <libnetq/sync/posix/Once.h>
#else
# include <libnetq/sync/stub/Once.h>
#endif

#endif /* _LIBNETQ_SYNC_ONCE_H */
