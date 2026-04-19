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

#include <libnetq/atomic/AtomicTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_ATOMIC32_INIT(counter) { counter }

static NQ_ALWAYS_INLINE void NQAtomic32_init(NQAtomic32* thiz, int32_t counter)
{
  thiz->counter = counter;
}

#define NQAtomic32_finalize(a) ((void)0)

NQ_EXPORT int32_t NQAtomic32_addFetch(NQAtomic32*, int32_t i);
NQ_EXPORT void NQAtomic32_inc(NQAtomic32*);
NQ_EXPORT void NQAtomic32_dec(NQAtomic32*);
NQ_EXPORT bool NQAtomic32_compareExchange(NQAtomic32*, int32_t* expected, int32_t desired);

NQ_EXPORT void NQCompilerFence(void);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ATOMIC_H */
