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
#include <openssl/sha.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQSHA1 SHA_CTX
#define NQSHA1_DIGEST_SIZE SHA_DIGEST_LENGTH
typedef uint8_t NQSHA1Digest[NQSHA1_DIGEST_SIZE];

NQ_EXPORT void NQSHA1_init(NQSHA1*);
NQ_EXPORT void NQSHA1_addBytes(NQSHA1*, const uint8_t* data, size_t size);
NQ_EXPORT void NQSHA1_checksum(NQSHA1*, uint8_t* digest);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_CRYPTO_SHA1_H */
