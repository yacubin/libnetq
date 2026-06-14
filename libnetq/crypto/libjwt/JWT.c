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

#ifdef NQCONFIG_USE_LIBJWT_JWT

#include <errno.h>
#include <jwt.h>

#include <libnetq/string/String.h>
#include <libnetq/Malloc.h>
#include <libnetq/ErrorCode.h>
#include <libnetq/Math.h>
#include <libnetq/Log.h>

struct NQJWT {
  jwt_alg_t algid;
  jwt_t* jwt;
  NQStringVec token;
};

static void clearToken(NQStringVec* token)
{
  if (token->characters != NULL) {
    jwt_free_str((char*)token->characters);
    token->characters = NULL;
    token->length = 0;
  }
}

NQJWT* NQJWT_create(const char* alg)
{
  jwt_alg_t algid = jwt_str_alg(alg);
  if (algid == JWT_ALG_INVAL) {
    NQ_LOGE("Unsupported algorithm '%s'", alg);
    return NULL;
  }

  NQJWT* thiz = (NQJWT*)NQMalloc(sizeof(*thiz));
  if (thiz == NULL) {
    return NULL;
  }

  int ret = jwt_new(&thiz->jwt);
  if (ret != 0) {
    NQ_LOGE("jwt_new failed - %i", ret);
    NQFree(thiz);
    return NULL;
  }

  thiz->algid = algid;
  thiz->token.characters = NULL;
  thiz->token.length = 0;

  return thiz;
}

NQJWT* NQJWT_parse(const char* token, const void* seckey, size_t sklen)
{
  jwt_t* jwt;
  int ret = jwt_decode(&jwt, token, (const uint8_t*)seckey, (int)sklen);
  if (ret != 0) {
    NQ_LOGE("jwt_decode failed - %i", ret);
    return NULL;
  }

  NQJWT* thiz = (NQJWT*)NQMalloc(sizeof(*thiz));
  if (thiz == NULL) {
    jwt_free(jwt);
    return NULL;
  }

  thiz->algid = jwt_get_alg(jwt);
  thiz->jwt = jwt;
  thiz->token.characters = NULL;
  thiz->token.length = 0;

  return thiz;
}

void NQJWT_release(NQJWT* thiz)
{
  clearToken(&thiz->token);
  jwt_free(thiz->jwt);
  NQFree(thiz);
}

bool NQJWT_headerGetBool(NQJWT* thiz, const char* name, bool* value)
{
  int val = jwt_get_header_bool(thiz->jwt, name);
  if ((val == 0 || val == -1) && errno != 0)
    return false;
  if (value)
    *value = val ? true : false;
  return true;
}

bool NQJWT_headerGetInt64(NQJWT* thiz, const char* name, int64_t* value)
{
  long val = jwt_get_header_int(thiz->jwt, name);
  if ((val == 0 || val == -1) && errno != 0)
    return false;
  if (value)
    *value = val;
  return true;
}

bool NQJWT_headerGetString(NQJWT* thiz, const char* name, const char** value)
{
  const char* val = jwt_get_header(thiz->jwt, name);
  if (val == NULL)
    return false;
  if (value)
    *value = val;
  return true;
}

bool NQJWT_headerSetBool(NQJWT* thiz, const char* name, bool value)
{
  if (NQJWTHeaderIsReserved(name))
    return false;

  clearToken(&thiz->token);
  return jwt_add_header_bool(thiz->jwt, name, value) == 0;
}

bool NQJWT_headerSetInt64(NQJWT* thiz, const char* name, int64_t value)
{
  if (NQJWTHeaderIsReserved(name))
    return false;

  clearToken(&thiz->token);
  return jwt_add_header_int(thiz->jwt, name, value) == 0;
}

bool NQJWT_headerSetString(NQJWT* thiz, const char* name, const char* value)
{
  if (NQJWTHeaderIsReserved(name))
    return false;

  clearToken(&thiz->token);
  return jwt_add_header(thiz->jwt, name, value) == 0;
}

bool NQJWT_claimGetBool(NQJWT* thiz, const char* name, bool* value)
{
  int val = jwt_get_grant_bool(thiz->jwt, name);
  if ((val == 0 || val == -1) && errno != 0)
    return false;
  if (value)
    *value = val ? true : false;
  return true;
}

bool NQJWT_claimGetInt64(NQJWT* thiz, const char* name, int64_t* value)
{
  long val = jwt_get_grant_int(thiz->jwt, name);
  if ((val == 0 || val == -1) && errno != 0)
    return false;
  if (value)
    *value = val;
  return true;
}

bool NQJWT_claimGetString(NQJWT* thiz, const char* name, const char** value)
{
  const char* val = jwt_get_grant(thiz->jwt, name);
  if (val == NULL)
    return false;
  if (value)
    *value = val;
  return true;
}

bool NQJWT_claimSetBool(NQJWT* thiz, const char* name, bool value)
{
  clearToken(&thiz->token);
  return jwt_add_grant_bool(thiz->jwt, name, value) == 0;
}

bool NQJWT_claimSetInt64(NQJWT* thiz, const char* name, int64_t value)
{
  clearToken(&thiz->token);
  return jwt_add_grant_int(thiz->jwt, name, value) == 0;
}

bool NQJWT_claimSetString(NQJWT* thiz, const char* name, const char* value)
{
  clearToken(&thiz->token);
  return jwt_add_grant(thiz->jwt, name, value) == 0;
}

bool NQJWT_sign(NQJWT* thiz, const void* seckey, size_t sklen)
{
  clearToken(&thiz->token);

  int ret = jwt_set_alg(thiz->jwt, thiz->algid, (const uint8_t*)seckey, (int)sklen);
  if (ret != 0) {
    NQ_LOGE("jwt_set_alg failed - %i", ret);
    return false;
  }

  return true;
}

int NQJWT_token(NQJWT* thiz, char* buffer, size_t length)
{
  if (thiz->token.characters == NULL) {
    thiz->token.characters = jwt_encode_str(thiz->jwt);
    if (thiz->token.characters == NULL)
      return -errno;
    thiz->token.length = NQStrlen(thiz->token.characters);
  }

  memcpy(buffer, thiz->token.characters, NQGetMin(thiz->token.length + 1, length));
  return (int)thiz->token.length;
}

#endif
