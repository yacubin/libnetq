/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/crypto/JWT.h"

#include <libnetq/string/String.h>

bool NQJWTTokenInfoParse(const char* token, struct NQJWTTokenInfo* result)
{
  result->header.characters = token;
  result->header.length = 0;

  while (true) {
    if(*token == '\0')
      return false;
    if (*token++ == '.')
      break;
    result->header.length++;
  }

  result->payload.characters = token;
  result->payload.length = 0;

  while (true) {
    if(*token == '\0')
      return false;
    if (*token++ == '.')
      break;
    result->payload.length++;
  }

  result->signature.characters = token;
  result->signature.length = 0;

  while (*token) {
    token++;
    result->signature.length++;
  }

  return true;
}

bool NQJWTHeaderIsReserved(const char* name)
{
  return NQStrcmp(name, NQ_JWT_HDR_ALG) == 0 || NQStrcmp(name, NQ_JWT_HDR_TYP) == 0;
}
