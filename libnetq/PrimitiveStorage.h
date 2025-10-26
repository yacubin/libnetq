/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_PRIMITIVESTORAGE_H
#define _LIBNETQ_PRIMITIVESTORAGE_H

#include <libnetq/Basic.h>
#include <libnetq/PrimitiveType.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQPrimitiveStorage NQPrimitiveStorage;

struct NQPrimitiveStorage {
  NQPrimitiveStorage* parent;
  struct NQPrimitiveStorageEntry* first;
};

NQ_EXPORT void NQPrimitiveStorage_init(NQPrimitiveStorage*, NQPrimitiveStorage* parent);
NQ_EXPORT void NQPrimitiveStorage_finalize(NQPrimitiveStorage*);

NQ_EXPORT size_t NQPrimitiveStorage_getUint32(NQPrimitiveStorage*, const char* name, uint32_t* value);
NQ_EXPORT bool NQPrimitiveStorage_setUint32(NQPrimitiveStorage*, const char* name, uint32_t value);

NQ_EXPORT size_t NQPrimitiveStorage_getString(NQPrimitiveStorage*, const char* name, char* buffer, size_t size);
NQ_EXPORT bool NQPrimitiveStorage_setString(NQPrimitiveStorage*, const char* name, const char* value);

typedef size_t (NQPrimitiveStorageHandler) (void* userdata, NQPrimitiveType type, char* buffer, size_t size);
NQ_EXPORT bool NQPrimitiveStorage_setHandle(NQPrimitiveStorage*, const char* name, NQPrimitiveStorageHandler* handler, void* userdata);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_PRIMITIVESTORAGE_H */
