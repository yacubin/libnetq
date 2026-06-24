/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_STUB_ATOMIC_H
#define _LIBNETQ_SYNC_STUB_ATOMIC_H

#include <libnetq/Basic.h>

typedef struct NQAtomic NQAtomic;
typedef struct NQAtomic64 NQAtomic64;

struct NQAtomic {
  int32_t counter;
};

struct NQAtomic64 {
  int64_t counter;
};

static inline int32_t __NQAtomic_addFetch(NQAtomic* thiz, int32_t i)
{
  thiz->counter += i;
  return thiz->counter;
}

static inline void __NQAtomic_inc(NQAtomic* thiz)
{
#if (defined(NQ_COMPILER_GCC) || defined(NQ_COMPILER_CLANG)) && (defined(NQ_CPU_X86) || defined(NQ_CPU_AMD64))
  asm volatile("lock incl %0" : "=m" (thiz->counter) :: "memory");

#elif (defined(NQ_COMPILER_GCC) || defined(NQ_COMPILER_CLANG)) && defined(NQ_CPU_ARM)
  asm volatile (
      "1:     ldrex   r1, [%0]\n"    // Load *ptr into r1, exclusive monitor
      "       add     r1, r1, #1\n"  // Increment
      "       strex   r2, r1, [%0]\n"// Try store; r2 = 0 on success
      "       cmp     r2, #0\n"      // Did store succeed?
      "       bne     1b\n"          // Retry if failed
      :
      : "r" (&thiz->counter)
      : "r1", "r2", "cc", "memory"
  );

#elif (defined(NQ_COMPILER_GCC) || defined(NQ_COMPILER_CLANG)) && defined(NQ_CPU_ARM64)
  asm volatile (
      "mov     w1, #1\n"              // Value to add
      "ldaddal w1, wzr, [%0]\n"      // Atomic add; discard old value
      :
      : "r" (&thiz->counter)
      : "w1", "memory"
  );

#else
  thiz->counter++;

#endif
}

static inline void __NQAtomic_dec(NQAtomic* thiz)
{
#if (defined(NQ_COMPILER_GCC) || defined(NQ_COMPILER_CLANG)) && (defined(NQ_CPU_X86) || defined(NQ_CPU_AMD64))
  asm volatile("lock decl %0" : "=m" (thiz->counter) :: "memory");

#elif (defined(NQ_COMPILER_GCC) || defined(NQ_COMPILER_CLANG)) && defined(NQ_CPU_ARM)
  asm volatile (
      "1: ldrex r1, [%0]\n"      // Load value from *ptr into r1 (exclusive)
      "   add   r1, r1, #1\n"    // Increment
      "   strex r2, r1, [%0]\n"  // Try to store; r2 = 0 on success
      "   cmp   r2, #0\n"        // Check if store succeeded
      "   bne   1b\n"            // Retry if failed
      : /* no outputs */
      : "r" (&thiz->counter)     // %0 = pointer to counter
      : "r1", "r2", "cc", "memory"
  );

#elif (defined(NQ_COMPILER_GCC) || defined(NQ_COMPILER_CLANG)) && defined(NQ_CPU_ARM64)
  asm volatile (
      "ldaddal %w1, %w1, [%0]\n" // Atomically add %w1 to *%0, return old value in %w1
      : /* no outputs */
      : "r" (&thiz->counter), "r" (1)
      : "memory"
  );

#else
  thiz->counter--;

#endif
}

static inline int32_t __NQAtomic_cmpxchg(NQAtomic* thiz, int32_t oldVal, int32_t newVal)
{
  int32_t prev = thiz->counter;
  if (prev == oldVal)
    thiz->counter = newVal;
  return prev;
}

static inline bool __NQAtomic_compareExchange(NQAtomic* thiz, int32_t* expected, int32_t desired)
{
  if (thiz->counter == *expected) {
    thiz->counter = desired;
    return true;
  }
  else {
    *expected = thiz->counter;
    return false;
  }
}

static inline void __NQCompilerFence(void)
{
  asm volatile("" ::: "memory");
}

static inline void __NQCompilerFence(void)
{
  asm volatile("" ::: "memory");
}

#endif /* _LIBNETQ_SYNC_STUB_ATOMIC_H */
