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

#ifdef NQCONFIG_USE_STUB_JWT

#include <libnetq/ErrorCode.h>

NQJWT* NQJWT_create(const char* alg)
{
  NQ_UNUSED_PARAM(alg);
  return NULL;
}

NQJWT* NQJWT_parse(const char* token, const void* seckey, size_t sklen)
{
  NQ_UNUSED_PARAM(token);
  NQ_UNUSED_PARAM(seckey);
  NQ_UNUSED_PARAM(sklen);
  return NULL;
}

void NQJWT_release(NQJWT* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

bool NQJWT_headerGetBool(NQJWT* thiz, const char* name, bool* value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_headerGetInt64(NQJWT* thiz, const char* name, int64_t* value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_headerGetString(NQJWT* thiz, const char* name, const char** value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_headerSetBool(NQJWT* thiz, const char* name, bool value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_headerSetInt64(NQJWT* thiz, const char* name, int64_t value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_headerSetString(NQJWT* thiz, const char* name, const char* value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_claimGetBool(NQJWT* thiz, const char* name, bool* value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_claimGetInt64(NQJWT* thiz, const char* name, int64_t* value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_claimSetBool(NQJWT* thiz, const char* name, bool value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_claimSetInt64(NQJWT* thiz, const char* name, int64_t value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_claimGetString(NQJWT* thiz, const char* name, const char** value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_claimSetString(NQJWT* thiz, const char* name, const char* value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);
  NQ_UNUSED_PARAM(value);
  return false;
}

bool NQJWT_sign(NQJWT* thiz, const void* seckey, size_t sklen)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(seckey);
  NQ_UNUSED_PARAM(sklen);
  return false;
}

int NQJWT_token(NQJWT* thiz, char* buffer, size_t length)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(buffer);
  NQ_UNUSED_PARAM(length);
  return -NQ_ENOTSUP;
}

#endif
