/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/ByteBufferWriter.h"

#define toByteBufferWriter(writer) NQ_CONTAINER_OF(writer, struct NQByteBufferWriter, base)

static int writeBuffer(NQIOWriter* writer, const void* data, size_t size)
{
  NQByteBufferWriter* thiz = toByteBufferWriter(writer);
  return NQByteBuffer_append(&thiz->bufferOwn, data, size) ? size : -1;
}

static void releaseBuffer(NQIOWriter* writer)
{
  NQByteBufferWriter* thiz = toByteBufferWriter(writer);
  NQByteBuffer_finalize(&thiz->bufferOwn);
}

static const struct NQIOWriterCallbacks s_callbacks =
{
  .write = writeBuffer,
  .flush = NQIOWriter_flushStub,
  .release = releaseBuffer,
};

void NQByteBufferWriter_init(NQByteBufferWriter* thiz)
{
  thiz->base.callbacks = &s_callbacks;
  NQByteBuffer_init(&thiz->bufferOwn);
}

static int writeBuffer2(NQIOWriter* writer, const void* data, size_t size)
{
  NQByteBufferWriter* thiz = toByteBufferWriter(writer);
  return NQByteBuffer_append(thiz->bufferPtr, data, size) ? size : -1;
}

static const struct NQIOWriterCallbacks s_callbacks2 =
{
  .write = writeBuffer2,
  .flush = NQIOWriter_flushStub,
  .release = NQIOWriter_releaseStub,
};

void NQByteBufferWriter_init2(NQByteBufferWriter* thiz, NQByteBuffer* buffer)
{
  thiz->base.callbacks = &s_callbacks2;
  thiz->bufferPtr = buffer;
}

const uint8_t* NQByteBufferWriter_data(const NQByteBufferWriter* thiz)
{
  return NQByteBuffer_data(thiz->base.callbacks == &s_callbacks ? &thiz->bufferOwn : thiz->bufferPtr);
}

size_t NQByteBufferWriter_size(const NQByteBufferWriter* thiz)
{
  return NQByteBuffer_size(thiz->base.callbacks == &s_callbacks ? &thiz->bufferOwn : thiz->bufferPtr);
}
