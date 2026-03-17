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

#ifdef NQ_USE_STUB_CRC32

uint32_t NQCRC32Calc(uint32_t crc, const uint8_t* data, size_t size)
{
  return 0;
}

#endif
