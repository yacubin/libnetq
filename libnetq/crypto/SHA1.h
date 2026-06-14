/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_SHA1_H
#define _LIBNETQ_CRYPTO_SHA1_H

#include <libnetq/Basic.h>

#if defined(NQCONFIG_USE_OPENSSL_SHA1)
# include <openssl/sha.h>
#else
# include <libnetq/crypto/CryptoHash.h>
#endif

#define NQSHA1_DIGEST_SIZE 20
typedef uint8_t NQSHA1Digest[NQSHA1_DIGEST_SIZE];

#ifdef __cplusplus
extern "C" {
#endif

#if defined(NQCONFIG_USE_OPENSSL_SHA1)

#define NQSHA1 SHA_CTX

NQ_EXPORT bool NQSHA1_init(NQSHA1*);
static inline void NQSHA1_finalize(NQSHA1* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}
NQ_EXPORT bool NQSHA1_update(NQSHA1*, const uint8_t* data, size_t size);
NQ_EXPORT bool NQSHA1_final(NQSHA1*, uint8_t* digest);

#else

typedef struct NQCryptoHash NQSHA1;

static inline bool NQSHA1_init(NQSHA1* thiz)
{
  // TODO: Check digestSize
  return NQCryptoHash_init(thiz, NQ_CRYPTOALGORITHM_SHA1);
}

static inline void NQSHA1_finalize(NQSHA1* thiz)
{
  NQCryptoHash_finalize(thiz);
}

static inline bool NQSHA1_update(NQSHA1* thiz, const uint8_t* data, size_t size)
{
  return NQCryptoHash_update(thiz, data, size);
}

static inline bool NQSHA1_final(NQSHA1* thiz, uint8_t* digest)
{
  return NQCryptoHash_final(thiz, digest);
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_CRYPTO_SHA1_H */
