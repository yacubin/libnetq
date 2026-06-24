/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_KERNEL_ONCE_H
#define _LIBNETQ_SYNC_KERNEL_ONCE_H

#include <libnetq/Basic.h>
#include <libnetq/sync/OnceCallback.h>

#ifdef NQ_OS_KERNEL

#include <linux/atomic.h>

typedef struct NQOnce NQOnce;

struct NQOnce {
  atomic_t flag;
};

#define NQ_ONCE_INIT { ATOMIC_INIT(0) }

static inline int NQOnce_call(NQOnce* once, NQOnceCallback callback)
{
  if (atomic_cmpxchg(&once->flag, 0, 1) == 0)
    callback();
  return 0;
}

#endif /* NQ_OS_KERNEL */
#endif /* _LIBNETQ_SYNC_KERNEL_ONCE_H */
