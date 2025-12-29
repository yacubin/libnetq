/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"

#if WITH_OPENSSL

#include <libnetq/crypto/SHA1.h>
#include <libnetq/Assert.h>

void NQSHA1_init(NQSHA1* thiz)
{
  int rc = SHA1_Init(thiz);
  NQ_ASSERT_UNUSED(rc, rc == 1);
}

void NQSHA1_addBytes(NQSHA1* thiz, const uint8_t* data, size_t size)
{
  int rc = SHA1_Update(thiz, data, size);
  NQ_ASSERT_UNUSED(rc, rc == 1);
}

void NQSHA1_checksum(NQSHA1* thiz, uint8_t* digest)
{
  int rc = SHA1_Final(digest, thiz);
  NQ_ASSERT_UNUSED(rc, rc == 1);
}

#endif /* WITH_OPENSSL */
