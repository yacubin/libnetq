/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/io/ByteBufferWriter.h"

#define toByteBufferWriter(writer) NQ_CONTAINER_OF(writer, struct NQByteBufferWriter, base)

static int writeBuffer(NQIOWriter* writer, const void* data, size_t size)
{
  NQByteBufferWriter* thiz = toByteBufferWriter(writer);
  return NQByteBuffer_append(&thiz->buffer, data, size) ? (int)size : -1;
}

static void releaseBuffer(NQIOWriter* writer)
{
  NQByteBufferWriter* thiz = toByteBufferWriter(writer);
  NQByteBuffer_finalize(&thiz->buffer);
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
  NQByteBuffer_init(&thiz->buffer);
}
