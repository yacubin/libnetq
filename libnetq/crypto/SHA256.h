/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_SHA256_H
#define _LIBNETQ_CRYPTO_SHA256_H

#include <libnetq/Basic.h>

#if defined(NQ_USE_OPENSSL_SHA256)
# include <openssl/sha.h>
#else
# include <libnetq/crypto/CryptoHash.h>
#endif

#define NQSHA256_DIGEST_SIZE 32
typedef uint8_t NQSHA256Digest[NQSHA256_DIGEST_SIZE];

#ifdef __cplusplus
extern "C" {
#endif

#if defined(NQ_USE_OPENSSL_SHA256)

#define NQSHA256 SHA256_CTX

NQ_EXPORT bool NQSHA256_init(NQSHA256*);

static inline void NQSHA256_finalize(NQSHA256* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

NQ_EXPORT bool NQSHA256_update(NQSHA256*, const uint8_t* data, size_t size);
NQ_EXPORT bool NQSHA256_final(NQSHA256*, uint8_t* digest);

#else

typedef struct NQCryptoHash NQSHA256;

static inline bool NQSHA256_init(NQSHA256* thiz)
{
  // TODO: Check digestSize
  return NQCryptoHash_init(thiz, NQ_CRYPTOALGORITHM_SHA256);
}

static inline void NQSHA256_finalize(NQSHA256* thiz)
{
  NQCryptoHash_finalize(thiz);
}

static inline bool NQSHA256_update(NQSHA256* thiz, const uint8_t* data, size_t size)
{
  return NQCryptoHash_update(thiz, data, size);
}

static inline bool NQSHA256_final(NQSHA256* thiz, uint8_t* digest)
{
  return NQCryptoHash_final(thiz, digest);
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_CRYPTO_SHA256_H */
