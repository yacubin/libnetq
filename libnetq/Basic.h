/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_BASIC_H
#define _LIBNETQ_BASIC_H

#include <libnetq/Export.h>
#include <libnetq/Types.h>
#include <libnetq/Compiler.h>

#ifdef __cplusplus
namespace NQ {
template<typename T, size_t SIZE> char(&getArrayLength(T(&)[SIZE]))[SIZE];
#if defined(NQ_COMPILER_GCC) || defined(NQ_COMPILER_CLANG)
template<typename T> char(&getArrayLength(T(&)[0]))[0];
#endif
} // namespace NQ
#define NQ_ARRAY_LENGTH(array) sizeof(::NQ::getArrayLength(array))
#else
#define NQ_ARRAY_LENGTH(a) (sizeof(a) / sizeof((a)[0]) / ((sizeof(a) % sizeof((a)[0])) ? 0 : 1))
#endif

#define NQ_STRINGIZE(exp) #exp
#define NQ_STRINGIZE_OF(exp) NQ_STRINGIZE(exp)

#define NQ_OFFSETOF(st, m) ((size_t)&(((st *)0)->m))

#ifdef __cplusplus
#define NQ_OBJECT_OFFSETOF(class, field) (reinterpret_cast<ptrdiff_t>(&(reinterpret_cast<class*>(0x4000)->field)) - 0x4000)
#define NQ_CAST_OFFSET(from, to) (reinterpret_cast<uintptr_t>(static_cast<to>((reinterpret_cast<from>(0x4000)))) - 0x4000)
#endif

#define NQ_EXIT_FAILURE EXIT_FAILURE
#define NQ_EXIT_SUCCESS EXIT_SUCCESS

#define NQ_ALIGN_UP(num, align) (((num) + ((align) - 1)) & ~((align) - 1))

#endif /* _LIBNETQ_BASIC_H */
