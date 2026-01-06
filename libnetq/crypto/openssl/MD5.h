/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_MD5_H
#define _LIBNETQ_CRYPTO_MD5_H

#include <libnetq/Basic.h>
#include <openssl/md5.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQMD5 MD5_CTX
#define NQMD5_DIGEST_SIZE MD5_DIGEST_LENGTH
typedef uint8_t NQMD5Digest[NQMD5_DIGEST_SIZE];

NQ_EXPORT void NQMD5_init(NQMD5*);
NQ_EXPORT void NQMD5_addBytes(NQMD5*, const uint8_t* data, size_t size);
NQ_EXPORT void NQMD5_checksum(NQMD5*, uint8_t* digest);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_CRYPTO_MD5_H */
