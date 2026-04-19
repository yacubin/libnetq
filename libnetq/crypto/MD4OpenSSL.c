/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/crypto/MD4.h"

#ifdef NQ_USE_OPENSSL_MD4

#include <libnetq/Assert.h>

NQ_STATIC_ASSERT(NQMD4_DIGEST_SIZE == MD4_DIGEST_LENGTH, "");

bool NQMD4_init(NQMD4* thiz)
{
  int rc = MD4_Init(thiz);
  return rc == 1;
}

bool NQMD4_update(NQMD4* thiz, const uint8_t* data, size_t size)
{
  int rc = MD4_Update(thiz, data, size);
  return rc == 1;
}

bool NQMD4_final(NQMD4* thiz, uint8_t digest[NQMD4_DIGEST_SIZE])
{
  int rc = MD4_Final(digest, thiz);
  return rc == 1;
}

#endif /* USE_OPENSSL_MD4 */
