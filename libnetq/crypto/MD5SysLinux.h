/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_MD5SYSLINUX_H
#define _LIBNETQ_CRYPTO_MD5SYSLINUX_H

#include <libnetq/Basic.h>

#include <crypto/hash.h>
#include <crypto/md5.h>

typedef struct NQMD5 NQMD5;
struct NQMD5 {
  struct crypto_shash* tfm;
  struct shash_desc desc;
};

#define NQMD5_DIGEST_SIZE MD5_DIGEST_SIZE
typedef uint8_t NQMD5Digest[NQMD5_DIGEST_SIZE];

NQ_EXPORT bool NQMD5_init(NQMD5*);
NQ_EXPORT void NQMD5_finalize(NQMD5*);
NQ_EXPORT bool NQMD5_update(NQMD5*, const uint8_t* data, size_t size);
NQ_EXPORT bool NQMD5_final(NQMD5*, uint8_t digest[NQMD5_DIGEST_SIZE]);

#endif /* _LIBNETQ_CRYPTO_MD5SYSLINUX_H */
