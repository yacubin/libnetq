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

#ifdef NQCONFIG_USE_LIBBASE58_BASE58

#include <libbase58.h>

#include <libnetq/Limits.h>
#include <libnetq/String.h> // For memmove
#include <libnetq/ErrorCode.h>

int NQBase58Encode(const void* inData, size_t inSize, char* outData, size_t outSize)
{
  if (!b58enc(outData, &outSize, inData, inSize))
    return -NQ_EINVAL;
  return outSize ? (int)outSize - 1 : 0;
}

int NQBase58Decode(const char* inData, size_t inSize, void* outData, size_t outSize)
{
  size_t res = outSize;
  if (!b58tobin(outData, &res, inData, inSize))
    return -NQ_EINVAL;
  memmove(outData, (uint8_t*)outData + outSize - res, res);
  return (int)res;
}

#endif /* USE_LIBBASE58_BASE58 */
