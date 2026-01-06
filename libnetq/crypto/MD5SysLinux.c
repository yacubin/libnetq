/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/crypto/MD5.h"

#ifdef NQ_SYS_LINUX

#include <crypto/md5.h>
#include <libnetq/Log.h>

bool NQMD5_init(NQMD5* thiz)
{
  struct crypto_shash* tfm = crypto_alloc_shash("md5", 0, CRYPTO_ALG_ASYNC);
  if (IS_ERR(tfm)) {
    NQ_LOGE("Error initializing crypto_shash of MD5");
    return false;
  }
  if (crypto_shash_digestsize(tfm) != NQMD5_DIGEST_SIZE) {
    NQ_LOGE("Wrong digest size of MD5");
    crypto_free_shash(tfm);
    return false;
  }
  thiz->desc.tfm = tfm;
  int ret = crypto_shash_init(&thiz->desc);
  if (ret) {
    NQ_LOGE("Error initializing hash_desc of MD5");
    crypto_free_shash(tfm);
    return false;
  }
  return true;
}

void NQMD5_finalize(NQMD5* thiz)
{
  crypto_free_shash(thiz->tfm);
}

bool NQMD5_update(NQMD5* thiz, const uint8_t* data, size_t size)
{
  int ret = crypto_shash_update(&thiz->desc, data, size);
  if (ret) {
    NQ_LOGE("Failed to update MD5");
    return false;
  }
  return true;
}

bool NQMD5_final(NQMD5* thiz, uint8_t digest[NQMD5_DIGEST_SIZE])
{
  int ret = crypto_shash_final(&thiz->desc, digest);
  if (ret) {
    NQ_LOGE("Failed to complete MD5 digest");
    return false;
  }
  return true;
}

#endif /* NQ_SYS_LINUX */
