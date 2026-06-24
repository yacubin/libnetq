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

#ifdef NQCONFIG_USE_CJOSE_JWT

#include <cjose/cjose.h>

#include <libnetq/string/String.h>
#include <libnetq/Malloc.h>
#include <libnetq/Math.h>
#include <libnetq/ErrorCode.h>
#include <libnetq/Log.h>

struct NQJWT {
  cjose_header_t* header;
  NQJSON* claims;
  cjose_jws_t* jws;
};

const char* errmsgSafe(const cjose_err* err)
{
  return err->message ? err->message : "Unknown";
}

NQJWT* NQJWT_create(const char* alg)
{
  cjose_err err;

  cjose_header_t* header = cjose_header_new(&err);
  if (header == NULL) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    return NULL;
  }

  if (!cjose_header_set(header, NQ_JWT_HDR_ALG, alg, &err)) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    cjose_header_release(header);
    return NULL;
  }

  if (!cjose_header_set(header, NQ_JWT_HDR_TYP, NQ_JWT_TYP_JWT, &err)) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    cjose_header_release(header);
    return NULL;
  }

  NQJSON* claims = NQJSON_createObjectRef();
  if (claims == NULL) {
    cjose_header_release(header);
    return NULL;
  }

  NQJWT* thiz = (NQJWT*)NQMalloc(sizeof(*thiz));
  if (thiz == NULL) {
    cjose_header_release(header);
    NQJSON_release(claims);
    return NULL;
  }

  thiz->header = header;
  thiz->claims = claims;
  thiz->jws = NULL;

  return thiz;
}

NQJWT* NQJWT_parse(const char* token, const void* seckey, size_t sklen)
{
  cjose_err err;

  cjose_jws_t* jws = cjose_jws_import(token, NQStrlen(token), &err);
  if (jws == NULL) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    return NULL;
  }

  cjose_jwk_t* jwk = cjose_jwk_create_oct_spec(seckey, sklen, &err);
  if (jwk == NULL) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    cjose_jws_release(jws);
    return false;
  }

  if (!cjose_jws_verify(jws, jwk, &err)) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    cjose_jwk_release(jwk);
    cjose_jws_release(jws);
    return false;
  }

  cjose_jwk_release(jwk);

  uint8_t* payloadData;
  size_t payloadSize;
  if (!cjose_jws_get_plaintext(jws, &payloadData, &payloadSize, &err)) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    cjose_jws_release(jws);
    return NULL;
  }

  NQJSON* claims = NQJSON_parse2(payloadData, payloadSize);
  if (claims == NULL) {
    cjose_jws_release(jws);
    return NULL;
  }

  NQJWT* thiz = (NQJWT*)NQMalloc(sizeof(*thiz));
  if (thiz == NULL) {
    NQJSON_release(claims);
    return NULL;
  }

  thiz->header = cjose_jws_get_protected(jws);
  thiz->claims = claims;
  thiz->jws = jws;

  return thiz;
}

void NQJWT_release(NQJWT* thiz)
{
  cjose_header_release(thiz->header);
  NQJSON_release(thiz->claims);
  if (thiz->jws != NULL)
    cjose_jws_release(thiz->jws);
  NQFree(thiz);
}

bool NQJWT_headerGetBool(NQJWT* thiz, const char* name, bool* value)
{
  json_t* item = json_object_get((json_t*)thiz->header, name);
  if (!json_is_boolean(item))
    return false;
  if (value)
    *value = json_boolean_value(item);
  return true;
}

bool NQJWT_headerGetInt64(NQJWT* thiz, const char* name, int64_t* value)
{
  json_t* item = json_object_get((json_t*)thiz->header, name);
  if (!json_is_integer(item))
    return false;
  if (value)
    *value = json_integer_value(item);
  return true;
}

bool NQJWT_headerGetString(NQJWT* thiz, const char* name, const char** value)
{
  cjose_err err;
  const char* result = cjose_header_get(thiz->header, name, &err);
  if (result == NULL) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    return false;
  }
  if (value)
    *value = result;
  return true;
}

bool NQJWT_headerSetBool(NQJWT* thiz, const char* name, bool value)
{
  if (NQJWTHeaderIsReserved(name))
    return false;

#if 0
  cjose_err err;
  if (!cjose_header_set_raw(thiz->header, name, value ? "true" : "false", &err)) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    return false;
  }
#else
  json_t* item = json_boolean(value);
  if (item == NULL)
    return false;
  json_object_set_new((json_t*)thiz->header, name, item);
#endif

  return true;
}

bool NQJWT_headerSetInt64(NQJWT* thiz, const char* name, int64_t value)
{
  if (NQJWTHeaderIsReserved(name))
    return false;

#if 0
  char buffer[24];
  snprintf(buffer, sizeof(buffer), "%llu", (unsigned long long)value);
  cjose_err err;
  if (!cjose_header_set_raw(thiz->header, name, buffer, &err)) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    return false;
  }
#else
  json_t* item = json_integer(value);
  if (item == NULL)
    return false;
  json_object_set_new((json_t*)thiz->header, name, item);
#endif

  return true;
}

bool NQJWT_headerSetString(NQJWT* thiz, const char* name, const char* value)
{
  if (NQJWTHeaderIsReserved(name))
    return false;

  cjose_err err;
  if (!cjose_header_set(thiz->header, name, value, &err)) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    return false;
  }

  return true;
}

bool NQJWT_claimGetBool(NQJWT* thiz, const char* name, bool* value)
{
  return NQJSON_objectGetBool(thiz->claims, name, value);
}

bool NQJWT_claimGetInt64(NQJWT* thiz, const char* name, int64_t* value)
{
  return NQJSON_objectGetInt64(thiz->claims, name, value);
}

bool NQJWT_claimGetString(NQJWT* thiz, const char* name, const char** value)
{
  return NQJSON_objectGetString(thiz->claims, name, value);
}

bool NQJWT_claimSetBool(NQJWT* thiz, const char* name, bool value)
{
  return NQJSON_objectSetBool(thiz->claims, name, value);
}

bool NQJWT_claimSetInt64(NQJWT* thiz, const char* name, int64_t value)
{
  return NQJSON_objectSetInt64(thiz->claims, name, value);
}

bool NQJWT_claimSetString(NQJWT* thiz, const char* name, const char* value)
{
  return NQJSON_objectSetString(thiz->claims, name, value);
}

bool NQJWT_sign(NQJWT* thiz, const void* seckey, size_t sklen)
{
  cjose_err err;
  cjose_jwk_t* jwk = cjose_jwk_create_oct_spec(seckey, sklen, &err);
  if (jwk == NULL) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    return false;
  }

  NQStringPrint buffer;
  NQStringPrint_init(&buffer);

  if (!NQJSON_dump(thiz->claims, &buffer)) {
    NQStringPrint_finalize(&buffer);
    cjose_jwk_release(jwk);
    return false;
  }

  const char* payloadData = NQStringPrint_characters(&buffer);
  size_t payloadSize = NQStringPrint_length(&buffer);
  cjose_jws_t* jws = cjose_jws_sign(jwk, thiz->header, payloadData, payloadSize, &err);
  if (jws == NULL) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
  }
  else {
    if (thiz->jws != NULL)
      cjose_jws_release(thiz->jws);
    thiz->jws = jws;
  }

  NQStringPrint_finalize(&buffer);
  cjose_jwk_release(jwk);
  return (jws != NULL);
}

int NQJWT_token(NQJWT* thiz, char* buffer, size_t length)
{
  cjose_err err;
  const char* token;
  if (!cjose_jws_export(thiz->jws, &token, &err)) {
    NQ_LOGE("%s (%i)", errmsgSafe(&err), err.code);
    return -NQ_ENOMEM;
  }

  size_t lenz = NQStrlen(token) + 1;
  if (lenz > length) {
    lenz = length;
  }

  memcpy(buffer, token, lenz);
  return (int)lenz - 1;
}

#endif
