/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ARRAY_H
#define _LIBNETQ_ARRAY_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQUint8Array NQUint8Array;
struct NQUint8Array {
  uint32_t size;
  uint8_t data[1];
};

NQ_EXPORT NQUint8Array* NQUint8Array_alloc(size_t size);
NQ_EXPORT NQUint8Array* NQUint8Array_fromFile(const char* filename);
NQ_EXPORT NQUint8Array* NQUint8Array_create(const uint8_t* data, size_t size);
NQ_EXPORT void NQUint8Array_destroy(NQUint8Array* array);

#define NQUint8Array_data(thiz) (thiz)->data

static inline size_t NQUint8Array_size(const NQUint8Array* thiz)
{
  return thiz->size;
}

static inline bool NQUint8Array_isEmpty(const NQUint8Array* thiz)
{
  return thiz->size == 0;
}

NQ_EXPORT NQUint8Array* NQUint8Array_copy(const NQUint8Array* array);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ARRAY_H */
