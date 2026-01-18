/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_IO_BYTEBUFFERWRITER_H
#define _LIBNETQ_IO_BYTEBUFFERWRITER_H

#include <libnetq/ByteBuffer.h>
#include <libnetq/io/IOWriter.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQByteBufferWriter NQByteBufferWriter;
struct NQByteBufferWriter {
  NQIOWriter base;
  NQByteBuffer buffer;
};

NQ_EXPORT void NQByteBufferWriter_init(NQByteBufferWriter*);

static inline void NQByteBufferWriter_finalize(NQByteBufferWriter* thiz)
{
  NQIOWriter_release(&thiz->base);
}

static inline int NQByteBufferWriter_write(NQByteBufferWriter* thiz, const void* data, size_t size)
{
  return NQIOWriter_write(&thiz->base, data, size);
}

#define NQByteBufferWriter_data(thiz) NQByteBuffer_data(&(thiz)->buffer)
#define NQByteBufferWriter_size(thiz) NQByteBuffer_size(&(thiz)->buffer)

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_IO_BYTEBUFFERWRITER_H */
