/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_CRYPTOHASHSYSLINUX_H
#define _LIBNETQ_CRYPTO_CRYPTOHASHSYSLINUX_H

#include <libnetq/Basic.h>

#ifdef NQ_SYS_LINUX

#include <crypto/hash.h>

typedef struct NQCryptoHash NQCryptoHash;
struct NQCryptoHash {
  struct crypto_shash* tfm;
  struct shash_desc desc;
};

NQ_EXPORT bool NQCryptoHash_init(NQCryptoHash*, uint32_t algId);
NQ_EXPORT void NQCryptoHash_finalize(NQCryptoHash*);
NQ_EXPORT bool NQCryptoHash_update(NQCryptoHash*, const uint8_t* data, size_t size);
NQ_EXPORT bool NQCryptoHash_final(NQCryptoHash*, uint8_t* digest);
NQ_EXPORT uint32_t NQCryptoHash_digestSize(const NQCryptoHash*);

#endif

#endif /* _LIBNETQ_CRYPTO_CRYPTOHASHSYSLINUX_H */
