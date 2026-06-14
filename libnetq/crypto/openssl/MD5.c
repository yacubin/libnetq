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

#ifdef NQCONFIG_USE_OPENSSL_MD5

#include <libnetq/Assert.h>

NQ_STATIC_ASSERT(NQMD5_DIGEST_SIZE == MD5_DIGEST_LENGTH, "");

bool NQMD5_init(NQMD5* thiz)
{
  int rc = MD5_Init(thiz);
  return rc == 1;
}

bool NQMD5_update(NQMD5* thiz, const uint8_t* data, size_t size)
{
  int rc = MD5_Update(thiz, data, size);
  return rc == 1;
}

bool NQMD5_final(NQMD5* thiz, uint8_t digest[NQMD5_DIGEST_SIZE])
{
  int rc = MD5_Final(digest, thiz);
  return rc == 1;
}

#endif /* USE_OPENSSL_MD5 */
