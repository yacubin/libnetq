/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_KERNEL_ATOMIC_H
#define _LIBNETQ_SYNC_KERNEL_ATOMIC_H

#include <libnetq/Basic.h>
#include <linux/atomic.h>

typedef atomic_t NQAtomic;
typedef atomic64_t NQAtomic64;

static NQ_ALWAYS_INLINE int32_t __NQAtomic_addFetch(NQAtomic* thiz, int32_t i)
{
  return atomic_add_return(i, thiz);
}

static NQ_ALWAYS_INLINE void __NQAtomic_inc(NQAtomic* thiz)
{
  atomic_inc(thiz);
}

static NQ_ALWAYS_INLINE void __NQAtomic_dec(NQAtomic* thiz)
{
  atomic_dec(thiz);
}

static NQ_ALWAYS_INLINE int32_t __NQAtomic_cmpxchg(NQAtomic* thiz, int32_t oldVal, int32_t newVal)
{
  return atomic_cmpxchg(thiz, oldVal, newVal);
}

static NQ_ALWAYS_INLINE bool __NQAtomic_compareExchange(NQAtomic* thiz, int32_t* expected, int32_t desired)
{
  int32_t old = atomic_cmpxchg(thiz, *expected, desired);
  if (old == *expected)
      return true;

  *expected = old;
  return false;
}

static NQ_ALWAYS_INLINE void __NQCompilerFence(void)
{
  barrier();
}

static NQ_ALWAYS_INLINE void __NQMemoryBarrier(void)
{
  smp_mb();
}

#endif /* _LIBNETQ_SYNC_KERNEL_ATOMIC_H */
