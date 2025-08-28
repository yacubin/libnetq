/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CONSTEXPR_H
#define _LIBNETQ_CONSTEXPR_H

#include <libnetq/Compiler.h>

#ifdef NQ_COMPILER_MSVC
#define NQ_INT32_C(c) c ## i32
#define NQ_UINT32_C(c) c ## ui32
#else
#define NQ_INT32_C(c) c
#define NQ_UINT32_C(c) c ## u
#endif

#ifdef NQ_COMPILER_MSVC
#define NQ_INT64_C(c) c ## i64
#define NQ_UINT64_C(c) c ## ui64
#elif defined(__WORDSIZE) && __WORDSIZE == 64
#define NQ_INT64_C(c) c ## l
#define NQ_UINT64_C(c) c ## ul
#elif defined(__WORDSIZE) && __WORDSIZE == 32
#define NQ_INT64_C(c) c ## ll
#define NQ_UINT64_C(c) c ## ull
#elif defined(__LP64__)
#define NQ_INT64_C(c) c ## l
#define NQ_UINT64_C(c) c ## ul
#else
#define NQ_INT64_C(c) c ## ll
#define NQ_UINT64_C(c) c ## ull
#endif

#endif /* _LIBNETQ_CONSTEXPR_H */
