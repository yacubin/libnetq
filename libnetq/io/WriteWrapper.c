/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/io/WriteWrapper.h"

bool NQWriteWrapper_writeAll(NQWriteWrapper* thiz, const void* data, size_t size)
{
  const uint8_t* ptr = (const uint8_t*)data;
  while (size != 0) {
    int n = NQWriteWrapper_write(thiz, ptr, size);
    if (n <= 0)
      return false;
    ptr += n;
    size -= n;
  }
  return true;
}
