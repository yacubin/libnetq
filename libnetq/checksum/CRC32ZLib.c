/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/checksum/CRC32.h"

#if USE_ZLIB_CRC32

#include <libnetq/Math.h>
#include <libnetq/Limits.h>

#include <zlib.h>

uint32_t NQCRC32Calc(uint32_t crc, const uint8_t* data, size_t size)
{
#if ZLIB_VERNUM >= 0x1290
  return (uint32_t)crc32_z(crc, data, size);
#else
  while (size != 0) {
    uInt n = NQGetMin(NQ_UINT32_MAX, size);
    crc = (uint32_t)crc32(crc, data, n);
    size -= n;
  }
  return crc;
#endif
}

#endif /* USE_ZLIB_CRC32 */
