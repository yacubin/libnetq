/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/random/CryptoRandom.h"

#ifdef NQ_USE_PLATFORM_CRYPTORAND

#include "libnetq/random/UnlimitedRandom.h"

int NQGetCryptoRandom(void* data, size_t size)
{
  return NQGetUnlimitedRandom(data, size);
}

#endif
