/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Base58.h"

#ifdef NQ_USE_STUB_BASE58

#include <libnetq/ErrorCode.h>

int NQBase58Encode(const void* inData, size_t inSize, char* outData, size_t outSize)
{
  NQ_UNUSED_PARAM(inData);
  NQ_UNUSED_PARAM(inSize);
  NQ_UNUSED_PARAM(outData);
  NQ_UNUSED_PARAM(outSize);
  return -NQ_ENOTSUP;
}

int NQBase58Decode(const char* inData, size_t inSize, void* outData, size_t outSize)
{
  NQ_UNUSED_PARAM(inData);
  NQ_UNUSED_PARAM(inSize);
  NQ_UNUSED_PARAM(outData);
  NQ_UNUSED_PARAM(outSize);
  return -NQ_ENOTSUP;
}

#endif /* USE_STUB_BASE58 */
