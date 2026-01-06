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

#include <libnetq/crypto/MD5.h>
#include <libnetq/Assert.h>

void NQMD5_init(NQMD5* thiz)
{
  int rc = MD5_Init(thiz);
  NQ_ASSERT_UNUSED(rc, rc == 1);
}

void NQMD5_addBytes(NQMD5* thiz, const uint8_t* data, size_t size)
{
  int rc = MD5_Update(thiz, data, size);
  NQ_ASSERT_UNUSED(rc, rc == 1);
}

void NQMD5_checksum(NQMD5* thiz, uint8_t* digest)
{
  int rc = MD5_Final(digest, thiz);
  NQ_ASSERT_UNUSED(rc, rc == 1);
}

#endif /* WITH_OPENSSL */
