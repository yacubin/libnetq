/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/crypto/SHA1.h"

#ifdef NQ_SYS_LINUX

#include <crypto/sha1.h>
#include <libnetq/Log.h>

bool NQSHA1_init(NQSHA1* thiz)
{
  struct crypto_shash* tfm = crypto_alloc_shash("sha1", 0, CRYPTO_ALG_ASYNC);
  if (IS_ERR(tfm)) {
    NQ_LOGE("Error initializing crypto_shash of SHA1");
    return false;
  }
  if (crypto_shash_digestsize(tfm) != NQSHA1_DIGEST_SIZE) {
    NQ_LOGE("Wrong digest size of SHA1");
    crypto_free_shash(tfm);
    return false;
  }
  thiz->desc.tfm = tfm;
  int ret = crypto_shash_init(&thiz->desc);
  if (ret) {
    NQ_LOGE("Error initializing hash_desc of SHA1");
    crypto_free_shash(tfm);
    return false;
  }
  return true;
}

void NQSHA1_finalize(NQSHA1* thiz)
{
  crypto_free_shash(thiz->tfm);
}

bool NQSHA1_update(NQSHA1* thiz, const uint8_t* data, size_t size)
{
  int ret = crypto_shash_update(&thiz->desc, data, size);
  if (ret) {
    NQ_LOGE("Failed to update SHA1");
    return false;
  }
  return true;
}

bool NQSHA1_final(NQSHA1* thiz, uint8_t digest[NQSHA1_DIGEST_SIZE])
{
  int ret = crypto_shash_final(&thiz->desc, digest);
  if (ret) {
    NQ_LOGE("Failed to complete SHA1 digest");
    return false;
  }
  return true;
}

#endif /* NQ_SYS_LINUX */
