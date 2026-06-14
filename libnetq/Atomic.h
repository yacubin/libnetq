/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ATOMIC_H
#define _LIBNETQ_ATOMIC_H

#include <libnetq/sync/Atomic.h>

#define NQ_ATOMIC_INIT(counter) { counter }

static NQ_ALWAYS_INLINE void NQAtomic_init(NQAtomic* thiz, int32_t counter)
{
  thiz->counter = counter;
}

#define NQAtomic_finalize(a) ((void)0)

static NQ_ALWAYS_INLINE int32_t NQAtomic_addFetch(NQAtomic* thiz, int32_t i)
{
  return __NQAtomic_addFetch(thiz, i);
}

static NQ_ALWAYS_INLINE void NQAtomic_inc(NQAtomic* thiz)
{
  __NQAtomic_inc(thiz);
}

static NQ_ALWAYS_INLINE void NQAtomic_dec(NQAtomic* thiz)
{
  __NQAtomic_dec(thiz);
}

static NQ_ALWAYS_INLINE int32_t NQAtomic_cmpxchg(NQAtomic* thiz, int32_t oldVal, int32_t newVal)
{
  return __NQAtomic_cmpxchg(thiz, oldVal, newVal);
}

static NQ_ALWAYS_INLINE bool NQAtomic_compareExchange(NQAtomic* thiz, int32_t* expected, int32_t desired)
{
  return __NQAtomic_compareExchange(thiz, expected, desired);
}

static NQ_ALWAYS_INLINE void NQCompilerFence(void)
{
  __NQCompilerFence();
}

static NQ_ALWAYS_INLINE void NQMemoryBarrier(void)
{
  __NQMemoryBarrier();
}

#endif /* _LIBNETQ_ATOMIC_H */
