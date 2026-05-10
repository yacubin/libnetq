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

#ifdef NQ_USE_KERNEL_CRYPTOHASH

#include <libnetq/Log.h>

bool NQCryptoHash_init(NQCryptoHash* thiz, uint32_t algId)
{
  const char* algName;
  switch (algId) {
  case NQ_CRYPTOALGORITHM_MD4:
    algName = "md4";
    break;
  case NQ_CRYPTOALGORITHM_MD5:
    algName = "md5";
    break;
  case NQ_CRYPTOALGORITHM_SHA1:
    algName = "sha1";
    break;
  case NQ_CRYPTOALGORITHM_SHA256:
    algName = "sha256";
    break;
  default:
    NQ_LOGE("Unknown crypt algorithm (%u)", algId);
    return false;
  }

  struct crypto_shash* tfm = crypto_alloc_shash(algName, 0, 0);
  if (IS_ERR(tfm)) {
    NQ_LOGE("Error initializing crypto_shash");
    return false;
  }
  thiz->desc.tfm = tfm;
  int ret = crypto_shash_init(&thiz->desc);
  if (ret) {
    NQ_LOGE("Error initializing hash_desc");
    crypto_free_shash(tfm);
    return false;
  }
  return true;
}

void NQCryptoHash_finalize(NQCryptoHash* thiz)
{
  crypto_free_shash(thiz->tfm);
}

bool NQCryptoHash_update(NQCryptoHash* thiz, const uint8_t* data, size_t size)
{
  int ret = crypto_shash_update(&thiz->desc, data, size);
  if (ret) {
    NQ_LOGE("Failed to update crypto_shash");
    return false;
  }
  return true;
}

bool NQCryptoHash_final(NQCryptoHash* thiz, uint8_t* digest)
{
  int ret = crypto_shash_final(&thiz->desc, digest);
  if (ret) {
    NQ_LOGE("Failed to complete digest of crypto_shash");
    return false;
  }
  return true;
}

uint32_t NQCryptoHash_digestSize(const NQCryptoHash* thiz)
{
  return crypto_shash_digestsize(thiz->desc.tfm);
}

#endif /* NQ_OS_KERNEL */
