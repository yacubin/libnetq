/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/crypto/BCrypt.h"

#ifdef NQCONFIG_USE_OPENSSL_BCRYPT

#include <openssl/evp.h>

#include <libnetq/string/String.h>
#include <libnetq/crypto/SecureErase.h>

#define NQ_BCRYPT_ITER 100000

static inline bool bcryptHashPassword(const char* password, const void* salt, void* hash)
{
  return PKCS5_PBKDF2_HMAC(password, NQStrlen(password), salt, NQ_BCRYPT_SALTSIZE,
      NQ_BCRYPT_ITER, EVP_sha256(), NQ_BCRYPT_HASHSIZE, hash) == 1;
}

bool NQBCryptHashPassword(const char* password, const void* salt, void* hash)
{
  return bcryptHashPassword(password, salt, hash);
}

bool NQBCryptVerifyPassword(const char* password, const void* salt, const void* hash)
{
  uint8_t buffer[NQ_BCRYPT_HASHSIZE];
  bool result = bcryptHashPassword(password, salt, buffer) && CRYPTO_memcmp(hash, buffer, NQ_BCRYPT_HASHSIZE) == 0;
  NQSecureErase(buffer, sizeof(buffer));
  return result;
}

#endif /* NQCONFIG_USE_OPENSSL_BCRYPT */
