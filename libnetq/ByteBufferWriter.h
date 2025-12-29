/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_BYTEBUFFERWRITER_H
#define _LIBNETQ_BYTEBUFFERWRITER_H

#include <libnetq/IOWriter.h>
#include <libnetq/ByteBuffer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQByteBufferWriter NQByteBufferWriter;
struct NQByteBufferWriter {
  NQIOWriter base;
  union {
    NQByteBuffer bufferOwn;
    NQByteBuffer* bufferPtr;
  };
};

NQ_EXPORT void NQByteBufferWriter_init(NQByteBufferWriter*);
NQ_EXPORT void NQByteBufferWriter_init2(NQByteBufferWriter*, NQByteBuffer* buffer);

static inline void NQByteBufferWriter_finalize(NQByteBufferWriter* thiz)
{
  NQIOWriter_release(&thiz->base);
}

static inline int NQByteBufferWriter_write(NQByteBufferWriter* thiz, const void* data, size_t size)
{
  return NQIOWriter_write(&thiz->base, data, size);
}

NQ_EXPORT const uint8_t* NQByteBufferWriter_data(const NQByteBufferWriter* thiz);
NQ_EXPORT size_t NQByteBufferWriter_size(const NQByteBufferWriter* thiz);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_BYTEBUFFERWRITER_H */
