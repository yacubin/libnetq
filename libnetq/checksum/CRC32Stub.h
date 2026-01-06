/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CHECKSUM_CRC32STUB_H
#define _LIBNETQ_CHECKSUM_CRC32STUB_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline uint32_t NQCRC32Calc(uint32_t crc, const uint8_t* data, size_t size)
{
  return crc;
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_CHECKSUM_CRC32STUB_H */
