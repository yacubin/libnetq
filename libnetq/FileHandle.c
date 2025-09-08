/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/FileHandle.h"

#include <libnetq/Limits.h>

int64_t NQFileReadn(NQFileHandle handle, uint8_t* data, int64_t size)
{
  int sz;
  int64_t n;

  if (size < 0)
    return -1;

  for (n = 0; n < size; data += sz, n += sz) {
    sz = NQFileRead(handle, data, (size_t)(size - n));
    if (sz == 0)
      break;
    if (sz < 0)
      return -1;
  }

  return n;
}

int64_t NQFileWriten(NQFileHandle handle, const uint8_t* data, int64_t size)
{
  int sz;
  int64_t n;

  if (size < 0)
    return -1;

  for (n = 0; n < size; data += sz, n += sz) {
    sz = NQFileWrite(handle, data, (size_t)(size - n));
    if (sz <= 0)
      return -1;
  }

  return n;
}
