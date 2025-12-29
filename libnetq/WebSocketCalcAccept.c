/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/WebSocketCalcAccept.h"

#include <libnetq/String.h>
#include <libnetq/Base64.h>
#include <libnetq/crypto/SHA1.h>
#include <libnetq/WebSocketTypes.h>

int NQWebSocketCalcAccept(const char* key, char* buf, size_t len)
{
  NQSHA1 sha1;
  NQSHA1Digest digest;

  NQSHA1_init(&sha1);
  NQSHA1_addBytes(&sha1, (const uint8_t*)key, strlen(key));
  NQSHA1_addBytes(&sha1, (const uint8_t*)NQ_WEBSOCKET_SEC_MAGIC, NQ_CSTR_LENGTH(NQ_WEBSOCKET_SEC_MAGIC));
  NQSHA1_checksum(&sha1, digest);

  char keyBase64[32];
  return NQBase64Encode(digest, digest + sizeof(digest), buf, buf + len, 0);
}
