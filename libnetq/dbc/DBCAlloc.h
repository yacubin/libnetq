/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DBC_DBCALLOC_H
#define _LIBNETQ_DBC_DBCALLOC_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (NQDBCAllocFn)(void* allocator, size_t size);
typedef void (NQDBCFreeFn)(void* allocator, void* ptr);

NQ_EXPORT void* NQDBCAllocDefault(void* allocator, size_t size);
NQ_EXPORT void NQDBCFreeDefault(void* allocator, void* ptr);
NQ_EXPORT void NQDBCFreeNope(void* allocator, void* ptr);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_DBC_DBCALLOC_H */
