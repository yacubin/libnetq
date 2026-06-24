/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_ATOMIC_H
#define _LIBNETQ_SYNC_ATOMIC_H

#include <libnetq/Basic.h>

#if defined(NQCONFIG_USE_ATOMIC)
#include <libnetq/sync/atomic/Atomic.h>
#elif defined(NQCONFIG_USE_SYNC_ATOMIC)
#include <libnetq/sync/sync/Atomic.h>
#elif defined(NQ_OS_WINDOWS)
#include <libnetq/sync/win32/Atomic.h>
#elif defined(NQ_OS_KERNEL)
#include <libnetq/sync/kernel/Atomic.h>
#else
#include <libnetq/sync/stub/Atomic.h>
#endif

#endif /* _LIBNETQ_SYNC_ATOMIC_H */
