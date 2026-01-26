/*
 * MIT License
 *
 * Copyright (c) 2022-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_BYTEBUFFER_H
#define _LIBNETQ_BYTEBUFFER_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQByteBuffer {
  uint8_t* data;
  uint32_t size;
  uint32_t capacity;
} NQByteBuffer;

NQ_EXPORT void NQByteBuffer_init(NQByteBuffer*);
NQ_EXPORT void NQByteBuffer_finalize(NQByteBuffer*);

NQ_EXPORT void NQByteBuffer_clear(NQByteBuffer*);
NQ_EXPORT bool NQByteBuffer_resize(NQByteBuffer*, size_t size);
NQ_EXPORT bool NQByteBuffer_insert(NQByteBuffer*, size_t position, const uint8_t* data, size_t size);
NQ_EXPORT bool NQByteBuffer_append(NQByteBuffer*, const uint8_t* data, size_t size);
NQ_EXPORT void NQByteBuffer_swap(NQByteBuffer*, NQByteBuffer* other);
NQ_EXPORT void NQByteBuffer_removeAt(NQByteBuffer*, size_t position, size_t size);

#define NQByteBuffer_data(thiz) (thiz)->data

static inline size_t NQByteBuffer_size(const NQByteBuffer* thiz)
{
  return thiz->size;
}

static inline bool NQByteBuffer_isEmpty(const NQByteBuffer* thiz)
{
  return thiz->size == 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_BYTEBUFFER_H */
