/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_STUB_CRYPTOHASH_H
#define _LIBNETQ_CRYPTO_STUB_CRYPTOHASH_H

#include <libnetq/Basic.h>

#ifdef NQCONFIG_USE_STUB_CRYPTOHASH

typedef struct NQCryptoHash NQCryptoHash;
struct NQCryptoHash {
  int dummy;
};

static inline bool NQCryptoHash_init(NQCryptoHash* thiz, uint32_t algId)
{
  return false;
}

static inline void NQCryptoHash_finalize(NQCryptoHash* thiz)
{
}

static inline bool NQCryptoHash_update(NQCryptoHash* thiz, const uint8_t* data, size_t size)
{
  return false;
}

static inline bool NQCryptoHash_final(NQCryptoHash* thiz, uint8_t* digest)
{
  return false;
}

static inline uint32_t NQCryptoHash_digestSize(const NQCryptoHash* thiz)
{
  return 0;
}

#endif

#endif /* _LIBNETQ_CRYPTO_STUB_CRYPTOHASH_H */
