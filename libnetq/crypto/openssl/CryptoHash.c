/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/crypto/CryptoHash.h"

#ifdef NQCONFIG_USE_OPENSSL_CRYPTOHASH

#include <libnetq/Log.h>

bool NQCryptoHash_init(NQCryptoHash* thiz, uint32_t algId)
{
  const EVP_MD* type;
  switch (algId) {
  case NQ_CRYPTOALGORITHM_MD4:
#ifdef OPENSSL_NO_MD4
    NQ_LOGE("There is no MD4 in the OPensSL library");
    return false;
#else
    type = EVP_md4();
    break;
#endif
  case NQ_CRYPTOALGORITHM_MD5:
    type = EVP_md5();
    break;
  case NQ_CRYPTOALGORITHM_SHA1:
    type = EVP_sha1();
    break;
  case NQ_CRYPTOALGORITHM_SHA256:
    type = EVP_sha256();
    break;
  default:
    NQ_LOGE("Unknown crypt algorithm (%u)", algId);
    return false;
  }

  EVP_MD_CTX* ctx = EVP_MD_CTX_create();
  if (ctx == NULL) {
    NQ_LOGE("EVP_MD_CTX_create failed");
    return false;
  }

  if (EVP_DigestInit_ex(ctx, type, NULL) != 1) {
    NQ_LOGE("EVP_DigestInit_ex failed");
    EVP_MD_CTX_destroy(ctx);
    return false;
  }

  thiz->ctx = ctx;
  return true;
}

void NQCryptoHash_finalize(NQCryptoHash* thiz)
{
  EVP_MD_CTX_destroy(thiz->ctx);
}

bool NQCryptoHash_update(NQCryptoHash* thiz, const uint8_t* data, size_t size)
{
  if (EVP_DigestUpdate(thiz->ctx, data, size) != 1) {
    NQ_LOGE("EVP_DigestUpdate failed");
    return false;
  }
  return true;
}

bool NQCryptoHash_final(NQCryptoHash* thiz, uint8_t* digest)
{
  unsigned rsize;
  if (EVP_DigestFinal_ex(thiz->ctx, digest, &rsize) != 1) {
    NQ_LOGE("EVP_DigestFinal_ex failed");
    return false;
  }
  return true;
}

#endif /* NQCONFIG_USE_ADVAPI32_CRYPTOHASH */
