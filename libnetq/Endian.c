/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Endian.h"

static const short s_littlendian = true;

bool NQIsLittleEndian()
{
  return ((const uint8_t*)&s_littlendian)[0];
}

bool NQIsBigEndian()
{
  return ((const uint8_t*)&s_littlendian)[1];
}
