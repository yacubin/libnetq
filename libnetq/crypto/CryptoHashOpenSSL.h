/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_CRYPTOHASHOPENSSL_H
#define _LIBNETQ_CRYPTO_CRYPTOHASHOPENSSL_H

#include <libnetq/Basic.h>

#ifdef NQ_USE_OPENSSL_CRYPTOHASH

#include <openssl/evp.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQCryptoHash NQCryptoHash;

struct NQCryptoHash {
  EVP_MD_CTX* ctx;
};

NQ_EXPORT bool NQCryptoHash_init(NQCryptoHash*, uint32_t algId);
NQ_EXPORT void NQCryptoHash_finalize(NQCryptoHash*);
NQ_EXPORT bool NQCryptoHash_update(NQCryptoHash*, const uint8_t* data, size_t size);
NQ_EXPORT bool NQCryptoHash_final(NQCryptoHash*, uint8_t* digest);

static inline uint32_t NQCryptoHash_digestSize(const NQCryptoHash* thiz)
{
  return EVP_MD_CTX_size(thiz->ctx);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

#endif /* _LIBNETQ_CRYPTO_CRYPTOHASHOPENSSL_H */
