/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/crypto/SHA256.h"

#ifdef NQ_USE_OPENSSL_SHA256

#include <libnetq/Assert.h>

NQ_STATIC_ASSERT(NQSHA256_DIGEST_SIZE == SHA256_DIGEST_LENGTH, "");

bool NQSHA256_init(NQSHA256* thiz)
{
  int rc = SHA256_Init(thiz);
  return rc == 1;
}

bool NQSHA256_update(NQSHA256* thiz, const uint8_t* data, size_t size)
{
  int rc = SHA256_Update(thiz, data, size);
  return rc == 1;
}

bool NQSHA256_final(NQSHA256* thiz, uint8_t* digest)
{
  int rc = SHA256_Final(digest, thiz);
  return rc == 1;
}

#endif /* USE_OPENSSL_SHA256 */
