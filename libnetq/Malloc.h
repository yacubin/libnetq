/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_MALLOC_H
#define _LIBNETQ_MALLOC_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT void* NQMalloc(size_t size);
NQ_EXPORT void* NQZeroMalloc(size_t size);
NQ_EXPORT void* NQCalloc(size_t numElements, size_t elementSize);
NQ_EXPORT void* NQRealloc(void*, size_t);
NQ_EXPORT void NQFree(void* ptr);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_MALLOC_H */
