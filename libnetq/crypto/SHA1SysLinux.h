/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_SHA1SYSLINUX_H
#define _LIBNETQ_CRYPTO_SHA1SYSLINUX_H

#include <libnetq/Basic.h>

#include <crypto/hash.h>
#include <crypto/sha1.h>

typedef struct NQSHA1 NQSHA1;
struct NQSHA1 {
  struct crypto_shash* tfm;
  struct shash_desc desc;
};

#define NQSHA1_DIGEST_SIZE SHA1_DIGEST_SIZE
typedef uint8_t NQSHA1Digest[NQSHA1_DIGEST_SIZE];

NQ_EXPORT bool NQSHA1_init(NQSHA1*);
NQ_EXPORT void NQSHA1_finalize(NQSHA1*);
NQ_EXPORT bool NQSHA1_update(NQSHA1*, const uint8_t* data, size_t size);
NQ_EXPORT bool NQSHA1_final(NQSHA1*, uint8_t digest[NQSHA1_DIGEST_SIZE]);

#endif /* _LIBNETQ_CRYPTO_SHA1SYSLINUX_H */
