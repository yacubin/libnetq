/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_BCRYPT_H
#define _LIBNETQ_CRYPTO_BCRYPT_H

#include <libnetq/random/CryptoRandom.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_BCRYPT_SALTSIZE 16
#define NQ_BCRYPT_HASHSIZE 32

static inline bool NQBCryptGenerateSalt(void* salt)
{
  return NQGetCryptoRandom(salt, NQ_BCRYPT_SALTSIZE) == 0;
}

NQ_EXPORT bool NQBCryptHashPassword(const char* password, const void* salt, void* hash);
NQ_EXPORT bool NQBCryptVerifyPassword(const char* password, const void* salt, const void* hash);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_CRYPTO_BCRYPT_H */
