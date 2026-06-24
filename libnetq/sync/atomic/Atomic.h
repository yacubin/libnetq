/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_ATOMIC_ATOMIC_H
#define _LIBNETQ_SYNC_ATOMIC_ATOMIC_H

#include <libnetq/Basic.h>

typedef struct NQAtomic NQAtomic;
typedef struct NQAtomic64 NQAtomic64;

struct NQAtomic {
  int32_t counter;
};

struct NQAtomic64 {
  int64_t counter;
};

static NQ_ALWAYS_INLINE int32_t __NQAtomic_addFetch(NQAtomic* thiz, int32_t i)
{
  return __atomic_add_fetch(&thiz->counter, i, __ATOMIC_SEQ_CST);
}

static NQ_ALWAYS_INLINE void __NQAtomic_inc(NQAtomic* thiz)
{
  (void)__atomic_add_fetch(&thiz->counter, 1, __ATOMIC_SEQ_CST);
}

static NQ_ALWAYS_INLINE void __NQAtomic_dec(NQAtomic* thiz)
{
  (void)__atomic_sub_fetch(&thiz->counter, 1, __ATOMIC_SEQ_CST);
}

static NQ_ALWAYS_INLINE int32_t __NQAtomic_cmpxchg(NQAtomic* thiz, int32_t oldVal, int32_t newVal)
{
  int32_t expected = oldVal;
  __atomic_compare_exchange_n(&thiz->counter, &expected, newVal, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  return expected;
}

static NQ_ALWAYS_INLINE bool __NQAtomic_compareExchange(NQAtomic* thiz, int32_t* expected, int32_t desired)
{
  return __atomic_compare_exchange_n(&thiz->counter, expected, desired, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

static NQ_ALWAYS_INLINE void __NQCompilerFence(void)
{
  __atomic_signal_fence(__ATOMIC_SEQ_CST);
}

static NQ_ALWAYS_INLINE void __NQMemoryBarrier(void)
{
  __atomic_thread_fence(__ATOMIC_SEQ_CST);
}

#endif /* _LIBNETQ_SYNC_ATOMIC_ATOMIC_H */
