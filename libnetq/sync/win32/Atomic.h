/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_WIN32_ATOMIC_H
#define _LIBNETQ_SYNC_WIN32_ATOMIC_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h>

#include <libnetq/Basic.h>

typedef struct NQAtomic NQAtomic;
typedef struct NQAtomic64 NQAtomic64;

struct NQAtomic {
  volatile LONG counter;
};

struct NQAtomic64 {
  volatile LONG64 counter;
};

static NQ_ALWAYS_INLINE int32_t __NQAtomic_addFetch(NQAtomic* thiz, int32_t i)
{
  return (int32_t)InterlockedExchangeAdd(&thiz->counter, i) + i;
}

static NQ_ALWAYS_INLINE void __NQAtomic_inc(NQAtomic* thiz)
{
  InterlockedIncrement(&thiz->counter);
}

static NQ_ALWAYS_INLINE void __NQAtomic_dec(NQAtomic* thiz)
{
  InterlockedDecrement(&thiz->counter);
}

static NQ_ALWAYS_INLINE int32_t __NQAtomic_cmpxchg(NQAtomic* thiz, int32_t oldVal, int32_t newVal)
{
  return (int32_t)(LONG)InterlockedCompareExchange(&thiz->counter, (LONG)newVal, (LONG)oldVal);
}

static NQ_ALWAYS_INLINE bool __NQAtomic_compareExchange(NQAtomic* thiz, int32_t* expected, int32_t desired)
{
  LONG old = InterlockedCompareExchange(&thiz->counter, desired, *expected);
  if (old == *expected)
    return true;

  *expected = old;
  return false;
}

static NQ_ALWAYS_INLINE void __NQCompilerFence(void)
{
#if defined(NQ_COMPILER_MSVC)
  _ReadWriteBarrier();
#else
  asm volatile("" ::: "memory");
#endif
}

static NQ_ALWAYS_INLINE void __NQMemoryBarrier(void)
{
  MemoryBarrier();
}

#endif /* _LIBNETQ_SYNC_WIN32_ATOMIC_H */
