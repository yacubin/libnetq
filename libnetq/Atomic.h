/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ATOMIC_H
#define _LIBNETQ_ATOMIC_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_ATOMIC32_INIT(counter) { counter }

typedef struct NQAtomic32 {
  volatile uint32_t counter;
} NQAtomic32;

typedef struct NQAtomic64 {
  volatile uint64_t counter;
} NQAtomic64;

NQ_EXPORT void NQAtomic32_init(NQAtomic32* thiz, uint32_t counter);
#define NQAtomic32_finalize(a) ((void)0)

NQ_EXPORT void NQAtomic32_inc(NQAtomic32* thiz);
NQ_EXPORT void NQAtomic32_dec(NQAtomic32* thiz);

NQ_EXPORT void NQCompilerFence(void);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ATOMIC_H */
