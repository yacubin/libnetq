/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_SHA1OPENSSL_H
#define _LIBNETQ_CRYPTO_SHA1OPENSSL_H

#include <libnetq/Basic.h>
#include <openssl/sha.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQSHA1 SHA_CTX
#define NQSHA1_DIGEST_SIZE SHA_DIGEST_LENGTH
typedef uint8_t NQSHA1Digest[NQSHA1_DIGEST_SIZE];

NQ_EXPORT bool NQSHA1_init(NQSHA1*);
static inline void NQSHA1_finalize(NQSHA1* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}
NQ_EXPORT bool NQSHA1_update(NQSHA1*, const uint8_t* data, size_t size);
NQ_EXPORT bool NQSHA1_final(NQSHA1*, uint8_t* digest);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_CRYPTO_SHA1OPENSSL_H */
