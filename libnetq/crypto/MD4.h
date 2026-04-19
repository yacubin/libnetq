/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_MD4_H
#define _LIBNETQ_CRYPTO_MD4_H

#include <libnetq/Basic.h>

#if defined(NQ_USE_OPENSSL_MD5)
# include <openssl/md4.h>
#else
# include <libnetq/crypto/CryptoHash.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define NQMD4_DIGEST_SIZE 16
typedef uint8_t NQMD4Digest[NQMD4_DIGEST_SIZE];

#if defined(NQ_USE_OPENSSL_MD4)

#define NQMD4 MD4_CTX

NQ_EXPORT bool NQMD4_init(NQMD4*);
static inline void NQMD4_finalize(NQMD4* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

NQ_EXPORT bool NQMD4_update(NQMD4*, const uint8_t* data, size_t size);
NQ_EXPORT bool NQMD4_final(NQMD4*, uint8_t digest[NQMD4_DIGEST_SIZE]);

#else

typedef struct NQCryptoHash NQMD4;

static inline bool NQMD4_init(NQMD4* thiz)
{
  // TODO: Check digestSize
  return NQCryptoHash_init(thiz, NQ_CRYPTOALGORITHM_MD4);
}

static inline void NQMD4_finalize(NQMD4* thiz)
{
  NQCryptoHash_finalize(thiz);
}

static inline bool NQMD4_update(NQMD4* thiz, const uint8_t* data, size_t size)
{
  return NQCryptoHash_update(thiz, data, size);
}

static inline bool NQMD4_final(NQMD4* thiz, uint8_t* digest)
{
  return NQCryptoHash_final(thiz, digest);
}

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_CRYPTO_MD4_H */
