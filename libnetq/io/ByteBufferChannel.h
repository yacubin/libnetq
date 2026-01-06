/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_IO_BYTEBUFFERCHANNEL_H
#define _LIBNETQ_IO_BYTEBUFFERCHANNEL_H

#include <libnetq/ByteBuffer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQByteBufferChannel NQByteBufferChannel;
struct NQByteBufferChannel {
  NQByteBuffer buffer;
  uint32_t position;
};

static inline void NQByteBufferChannel_init(NQByteBufferChannel* thiz)
{
  NQByteBuffer_init(&thiz->buffer);
  thiz->position = 0;
}

static inline void NQByteBufferChannel_finalize(NQByteBufferChannel* thiz)
{
  NQByteBuffer_finalize(&thiz->buffer);
}

#define NQByteBufferChannel_data(thiz) NQByteBuffer_data(&(thiz)->buffer)
#define NQByteBufferChannel_size(thiz) NQByteBuffer_size(&(thiz)->buffer)

NQ_EXPORT size_t NQByteBufferChannel_read(NQByteBufferChannel*, void* data, size_t size);
NQ_EXPORT size_t NQByteBufferChannel_write(NQByteBufferChannel*, const void* data, size_t size);

static inline bool NQByteBufferChannel_readAll(NQByteBufferChannel* thiz, void* data, size_t size)
{
  return NQByteBufferChannel_read(thiz, data, size) == size;
}

static inline bool NQByteBufferChannel_writeAll(NQByteBufferChannel* thiz, const void* data, size_t size)
{
  return NQByteBufferChannel_write(thiz, data, size) == size;
}

static inline size_t NQByteBufferChannel_position(const NQByteBufferChannel* thiz)
{
  return thiz->position;
}

static inline bool NQByteBufferChannel_seekTo(NQByteBufferChannel* thiz, size_t position)
{
  if (NQByteBuffer_size(&thiz->buffer) < position)
    return false; // Not Implemented
  thiz->position = position;
  return true;
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_IO_BYTEBUFFERCHANNEL_H */
