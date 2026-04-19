/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DBC_DBCWRITER_H
#define _LIBNETQ_DBC_DBCWRITER_H

#include <libnetq/dbc/DBCAlloc.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQDBCDocument NQDBCDocument;
typedef struct NQDBCDocWriter NQDBCDocWriter;

NQ_EXPORT NQDBCDocWriter* NQDBCDocWriter_create(const char* name);
NQ_EXPORT NQDBCDocWriter* NQDBCDocWriter_create2(const char* name, void* allocator, NQDBCAllocFn* alloc, NQDBCFreeFn* free);
NQ_EXPORT void NQDBCDocWriter_destroy(NQDBCDocWriter*);

NQ_EXPORT bool NQDBCDocWriter_write(NQDBCDocWriter*, const void* data, size_t size);
NQ_EXPORT NQDBCDocument* NQDBCDocWriter_toDocument(NQDBCDocWriter*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_DBC_DBCDOCWRITER_H */
