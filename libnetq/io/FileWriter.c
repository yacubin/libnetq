/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/io/FileWriter.h"

#include <libnetq/Log.h>
#include <libnetq/Malloc.h>

static int writeFile(NQIOWriter* writer, const void* data, size_t size)
{
  NQFileWriter* thiz = NQ_CONTAINER_OF(writer, struct NQFileWriter, base);
  return NQFileWrite(thiz->handle, (const uint8_t*)data, size);
}

static void destroyFile(NQIOWriter* writer)
{
  NQFileWriter* thiz = NQ_CONTAINER_OF(writer, struct NQFileWriter, base);
  NQFileClose(thiz->handle);
  NQFree(thiz);
}

static const struct NQIOWriterCallbacks s_ownFileCallbacks =
{
  .write = writeFile,
  .flush = NQIOWriter_flushStub,
  .release = destroyFile,
};

NQFileWriter* NQFileWriter_create(const char* filename)
{
  NQFileWriter* thiz = (NQFileWriter*)NQMalloc(sizeof(*thiz));
  if (thiz == NULL) {
    NQ_LOGE("No memory");
    return NULL;
  }
  thiz->handle = NQFileOpen(filename, NQ_FOPEN_WRITE);
  if (NQIsFileInvalid(thiz->handle)) {
    NQ_LOGE("File '%s' not exists", filename);
    NQFree(thiz);
    return NULL;
  }
  thiz->base.callbacks = &s_ownFileCallbacks;
  return thiz;
}

void NQFileWriter_destroy(NQFileWriter* thiz)
{
  NQFileWriter_finalize(thiz);
  NQFree(thiz);
}

static void finalizeFile(NQIOWriter* writer)
{
  NQFileWriter* thiz = NQ_CONTAINER_OF(writer, struct NQFileWriter, base);
  NQFileClose(thiz->handle);
}

static const struct NQIOWriterCallbacks s_fileCallbacks =
{
  .write = writeFile,
  .flush = NQIOWriter_flushStub,
  .release = finalizeFile,
};

bool NQFileWriter_init(NQFileWriter* thiz, const char* filename)
{
  thiz->handle = NQFileOpen(filename, NQ_FOPEN_WRITE);
  if (NQIsFileInvalid(thiz->handle)) {
    NQ_LOGE("File '%s' not exists", filename);
    return false;
  }
  thiz->base.callbacks = &s_fileCallbacks;
  return true;
}

void NQFileWriter_finalize(NQFileWriter* thiz)
{
  NQFileClose(thiz->handle);
}

int NQFileWriter_write(NQFileWriter* thiz, const void* data, size_t size)
{
  return NQFileWrite(thiz->handle, (const uint8_t*)data, size);
}
