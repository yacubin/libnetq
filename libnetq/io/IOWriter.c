/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/io/IOWriter.h"

#include <libnetq/io/FileWriter.h>

NQIOWriter* NQIOWriter_fromFile(const char* filename)
{
  NQFileWriter* writer = NQFileWriter_create(filename);
  if (writer == NULL)
    return NULL;
  return &writer->base;
}

bool NQIOWriter_writeAll(NQIOWriter* thiz, const void* data, size_t size)
{
  const uint8_t* ptr = (const uint8_t*)data;
  while (size != 0) {
    int n = NQIOWriter_write(thiz, ptr, size);
    if (n <= 0)
      return false;
    ptr += n;
    size -= n;
  }
  return true;
}

int NQIOWriter_flushStub(NQIOWriter* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return 0;
}

void NQIOWriter_releaseStub(NQIOWriter* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}
