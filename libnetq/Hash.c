/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Hash.h"

uint32_t NQHashCStringWithSeed(uint32_t hash, const char* characters)
{
  const uint8_t* p;
  for (p = (const uint8_t*)characters; *p; p++)
    hash = hash * 33 + *p;
  return hash;
}

uint32_t NQHashStringWithSeed(uint32_t hash, const char* characters, size_t length)
{
  const uint8_t* p;
  for (p = (const uint8_t*)characters; length != 0; length--, p++)
    hash = hash * 33 + *p;
  return hash;
}
