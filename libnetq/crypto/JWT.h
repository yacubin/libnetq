/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_JWT_H
#define _LIBNETQ_WEB_JWT_H

#include <libnetq/json/JSON.h>
#include <libnetq/string/StringRange.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQJWT NQJWT;

#define NQ_JWT_HDR_ALG "alg"
#define NQ_JWT_HDR_TYP "typ"
#define NQ_JWT_HDR_CTY "cty"
#define NQ_JWT_HDR_ENC "enc"

#define NQ_JWT_ALG_NONE  "none"
#define NQ_JWT_ALG_HS256 "HS256"
#define NQ_JWT_ALG_HS384 "HS384"
#define NQ_JWT_ALG_HS512 "HS512"

#define NQ_JWT_TYP_JWT "JWT"
#define NQ_JWT_CTY_JWT "JWT"

/* Issuer Claim */
#define NQ_JWT_CLM_ISS "iss"

/* Subject Claim */
#define NQ_JWT_CLM_SUB "sub"

/* Audience Claim */
#define NQ_JWT_CLM_AUD "aud"

/* Expiration Time Claim */
#define NQ_JWT_CLM_EXP "exp"

/* Not Before Claim */
#define NQ_JWT_CLM_NBF "nbf"

/* Issued At Claim */
#define NQ_JWT_CLM_IAT "iat"

/* JWT ID Claim */
#define NQ_JWT_CLM_JTI "jti"

NQ_EXPORT NQJWT* NQJWT_create(const char* alg);
NQ_EXPORT NQJWT* NQJWT_parse(const char* token, const void* seckey, size_t sklen);
// NQJWT* NQJWT_parse(const char* token);
NQ_EXPORT void NQJWT_release(NQJWT*);

NQ_EXPORT bool NQJWT_headerGetBool(NQJWT*, const char* name, bool* value);
NQ_EXPORT bool NQJWT_headerGetInt64(NQJWT*, const char* name, int64_t* value);
NQ_EXPORT bool NQJWT_headerGetString(NQJWT*, const char* name, const char** value);

NQ_EXPORT bool NQJWT_headerSetBool(NQJWT*, const char* name, bool value);
NQ_EXPORT bool NQJWT_headerSetInt64(NQJWT*, const char* name, int64_t value);
NQ_EXPORT bool NQJWT_headerSetString(NQJWT*, const char* name, const char* value);

NQ_EXPORT bool NQJWT_claimGetBool(NQJWT*, const char* name, bool* value);
NQ_EXPORT bool NQJWT_claimGetInt64(NQJWT*, const char* name, int64_t* value);
NQ_EXPORT bool NQJWT_claimGetString(NQJWT*, const char* name, const char** value);

NQ_EXPORT bool NQJWT_claimSetBool(NQJWT*, const char* name, bool value);
NQ_EXPORT bool NQJWT_claimSetInt64(NQJWT*, const char* name, int64_t value);
NQ_EXPORT bool NQJWT_claimSetString(NQJWT*, const char* name, const char* value);

NQ_EXPORT bool NQJWT_sign(NQJWT*, const void* seckey, size_t sklen);
// bool NQJWT_verify(const NQJWT*, const void* seckey, size_t sklen);
NQ_EXPORT int NQJWT_token(NQJWT*, char* buffer, size_t length);

typedef struct NQJWTTokenInfo NQJWTTokenInfo;
struct NQJWTTokenInfo {
  NQStringRange header;
  NQStringRange payload;
  NQStringRange signature;
};

NQ_EXPORT bool NQJWTTokenInfoParse(const char* token, struct NQJWTTokenInfo* result);

#define NQJWTTokenInfo_tokenData(thiz) ((thiz)->header.characters)
static inline size_t NQJWTTokenInfo_tokenSize(struct NQJWTTokenInfo* thiz)
{
  return thiz->signature.characters - thiz->header.characters + thiz->signature.length;
}

#define NQJWTTokenInfo_signingData(thiz) ((thiz)->header.characters)
static inline size_t NQJWTTokenInfo_signingSize(struct NQJWTTokenInfo* thiz)
{
  return thiz->payload.characters - thiz->header.characters + thiz->payload.length;
}

NQ_EXPORT bool NQJWTHeaderIsReserved(const char* name);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_JWT_H */
