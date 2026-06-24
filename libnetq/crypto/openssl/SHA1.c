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

#ifdef NQCONFIG_USE_OPENSSL_SHA1

#include <libnetq/Assert.h>

NQ_STATIC_ASSERT(NQSHA1_DIGEST_SIZE == SHA_DIGEST_LENGTH, "");

bool NQSHA1_init(NQSHA1* thiz)
{
  int rc = SHA1_Init(thiz);
  return rc == 1;
}

bool NQSHA1_update(NQSHA1* thiz, const uint8_t* data, size_t size)
{
  int rc = SHA1_Update(thiz, data, size);
  return rc == 1;
}

bool NQSHA1_final(NQSHA1* thiz, uint8_t* digest)
{
  int rc = SHA1_Final(digest, thiz);
  return rc == 1;
}

#endif /* USE_OPENSSL_SHA1 */
