/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_BASIC_H
#define _LIBNETQ_BASIC_H

#include <libnetq/Features.h>
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

#if NQ_HAS_BUILTIN(__builtin_offsetof)
#define NQ_OFFSETOF(type, field) __builtin_offsetof(type, field)
#elif defined(__cplusplus)
#define NQ_OFFSETOF(type, field) (reinterpret_cast<size_t>(&(reinterpret_cast<type*>(0x4000)->field)) - 0x4000)
#else
#define NQ_OFFSETOF(type, field) ((size_t)&(((type*)0)->field))
#endif

#define NQ_CONTAINER_OF(obj, type, field) ((type*)((char*)(obj) - NQ_OFFSETOF(type, field)))

#ifdef __cplusplus
#define NQ_CAST_OFFSET(from, to) (reinterpret_cast<uintptr_t>(static_cast<to>((reinterpret_cast<from>(0x4000)))) - 0x4000)
#endif

#define NQ_EXIT_FAILURE EXIT_FAILURE
#define NQ_EXIT_SUCCESS EXIT_SUCCESS

#define NQ_ALIGN_UP(num, align) (((num) + ((align) - 1)) & ~((align) - 1))

#define NQ_CONCAT2(a, b) a ## b
#define NQ_CONCAT(a, b) NQ_CONCAT2(a, b)

#endif /* _LIBNETQ_BASIC_H */
