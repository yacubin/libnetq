/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Atomic.h"

#include <libnetq/Compiler.h>
#include <libnetq/OS.h>
#include <libnetq/CPU.h>

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#endif

void NQAtomic32_init(NQAtomic32* thiz, uint32_t counter)
{
  thiz->counter = counter;
}

void NQAtomic32_inc(NQAtomic32* thiz)
{
#if defined(HAVE_INTEL_ATOMIC_PRIMITIVES)
  (void)__sync_fetch_and_add(&thiz->counter, 1);

#elif (defined(NQ_COMPILER_GCC) || defined(NQ_COMPILER_CLANG)) && (defined(NQ_CPU_X86) || defined(NQ_CPU_AMD64))
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

#elif defined(NQ_OS_WINDOWS)
  InterlockedIncrement(&thiz->counter);

#endif
}

void NQAtomic32_dec(NQAtomic32* thiz)
{
#if defined(HAVE_INTEL_ATOMIC_PRIMITIVES)
  (void)__sync_sub_and_fetch(&thiz->counter, 1);

#elif (defined(NQ_COMPILER_GCC) || defined(NQ_COMPILER_CLANG)) && (defined(NQ_CPU_X86) || defined(NQ_CPU_AMD64))
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

#elif defined(NQ_OS_WINDOWS)
  InterlockedDecrement(&thiz->counter);

#endif
}

void NQCompilerFence()
{
#if defined(NQ_OS_WIN) && !defined(NQ_COMPILER_GCC)
  _ReadWriteBarrier();
#else
  asm volatile("" ::: "memory");
#endif
}
