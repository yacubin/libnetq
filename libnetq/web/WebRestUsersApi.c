/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/web/WebRestUsersApi.h"

#include <libnetq/Malloc.h>
#include <libnetq/Time.h>
#include <libnetq/Sprintf.h>
#include <libnetq/string/StringPrint.h>
#include <libnetq/json/JSON.h>
#include <libnetq/web/JsonRpcResponse.h>
#include <libnetq/sql/UserDataStore.h>
#include <libnetq/crypto/JWT.h>
#include <libnetq/web/WebRequest.h>
#include <libnetq/web/WebResponse.h>
#include <libnetq/HttpHeader.h>
#include <libnetq/MediaType.h>
#include <libnetq/ErrorCode.h>
#include <libnetq/Log.h>

struct NQWebRestUsersApi {
  NQWebExecutor executor;
  struct NQWebRequestListener signupListener;
  struct NQWebRequestListener loginListener;
  NQSQLiteDatabase* database;
};

struct NQWebUserApiRequestModule {
  struct NQWebServerModuleOperations* module;
  NQSQLiteDatabase* database;
};

struct UserApiRequest {
  NQSQLiteDatabase* database;
  NQStringPrint recvBuffer;
};

static bool jsonWriterHandler(void* userdata, const char* characters, size_t size)
{
  NQWebResponse* response = (NQWebResponse*)userdata;
  int n = NQWebResponse_write(response, characters, size);
  return n < 0 ? false : true;
}

struct UserSignup {
  NQJSON* json;
  const char* name;
  const char* email;
  const char* password;
};

static void UserSignup_init(struct UserSignup* thiz)
{
  thiz->json = NULL;
  thiz->name = NULL;
  thiz->email = NULL;
  thiz->password = NULL;
}

static void UserSignup_finalize(struct UserSignup* thiz)
{
  if (thiz->json != NULL) {
    NQJSON_release(thiz->json);
  }
}

static bool UserSignup_parse(struct UserSignup* thiz, const char* characters, size_t length, bool nameRequired)
{
  UserSignup_finalize(thiz);
  UserSignup_init(thiz);

  thiz->json = NQJSON_parse2(characters, length);
  if (thiz->json == NULL) {
    NQ_LOGE("Invalid JSON");
    return false;
  }

  if (!nameRequired)
    thiz->name = NULL;
  else if (!NQJSON_objectGetString(thiz->json, "name", &thiz->name)) {
    NQ_LOGE("Missing required field: name");
    return false;
  }

  if (!NQJSON_objectGetString(thiz->json, "email", &thiz->email)) {
    NQ_LOGE("Missing required field: email");
    return false;
  }

  if (!NQJSON_objectGetString(thiz->json, "password", &thiz->password)) {
    NQ_LOGE("Missing required field: password");
    return false;
  }

  return true;
}

struct JWTClaims {
  int64_t iss;
  int64_t exp;
  uint32_t sub;
};

static bool buildJWT(struct JWTClaims* claims, const void* seckey, size_t sklen, char* buf, size_t len)
{
  int n;
  NQJWT* jwt = NQJWT_create(NQ_JWT_ALG_HS256);
  if (jwt == NULL) {
    NQ_LOGE("Failed to create JWT object");
    return false;
  }

  if (!NQJWT_claimSetInt64(jwt, NQ_JWT_CLM_ISS, claims->iss)) {
    NQ_LOGE("Failed to set JWT 'iss' claim");
    return false;
  }

  if (!NQJWT_claimSetInt64(jwt, NQ_JWT_CLM_EXP, claims->exp)) {
    NQ_LOGE("Failed to set JWT 'exp' claim");
    return false;
  }

  n = NQSnprintf(buf, len, "%u", claims->sub);
  if (n < 0 || len <= (size_t)n) {
    NQ_LOGE("Failed to format JWT 'sub' claim");
    return false;
  }

  if (!NQJWT_claimSetString(jwt, NQ_JWT_CLM_SUB, buf)) {
    NQ_LOGE("Failed to set JWT 'sub' claim");
    return false;
  }

  if (!NQJWT_sign(jwt, seckey, sklen)) {
    NQ_LOGE("Failed to sign JWT");
    return false;
  }

  n = NQJWT_token(jwt, buf, len);
  if (n < 0 || len <= (size_t)n) {
    NQ_LOGE("Failed to generate JWT token string");
    return false;
  }

  return true;
}

static int commonInit(NQWebRequest* request, void* data)
{
  struct NQWebRestUsersApi* userApi = (NQWebRestUsersApi*)data;
  struct UserApiRequest* uas = (struct UserApiRequest*)NQMalloc(sizeof(*uas));
  if (uas == NULL)
    return -NQ_ENOMEM;
  uas->database = userApi->database;
  NQStringPrint_init(&uas->recvBuffer);
  request->userdata = uas;
  return 0;
}

static size_t commonPostReceive(NQWebRequest* request, const char* data, size_t size)
{
  struct UserApiRequest* uas = (struct UserApiRequest*)request->userdata;
  return NQStringPrint_writeAll(&uas->recvBuffer, data, size) ? size : 0;
}

static int commonPostResponse(struct UserApiRequest* uas, const char* email, NQWebResponse* response)
{
  NQWebServer* server = NQWebResponse_server(response);

  struct JWTClaims claims;
  if (!NQUserDataStoreUserId(uas->database, email, &claims.sub)) {
    NQWebResponse_writeJsonRpcErrorParams(response, NQ_HTTP_INTERNAL_SERVER_ERROR, "Unable to get user id");
    return NQ_HTTP_INTERNAL_SERVER_ERROR;
  }

  claims.iss = NQGetTimeSec();
  claims.exp = claims.iss + 1 * NQ_HOURS_PER_DAY * NQ_MINS_PER_HOUR * NQ_SECS_PER_MIN; // 1d

  char buffer[256];
  if (!buildJWT(&claims, server->sessionSeckey, sizeof(server->sessionSeckey), buffer, sizeof(buffer))) {
    NQWebResponse_writeJsonRpcErrorParams(response, NQ_HTTP_INTERNAL_SERVER_ERROR, "Unable to create JWT");
    return NQ_HTTP_INTERNAL_SERVER_ERROR;
  }

  NQJSONWriter writer;
  NQJSONWriter_init(&writer, jsonWriterHandler, response);
  NQJSONWriter_writeObjectBegin(&writer);
  NQJSONWriter_writeKeyString(&writer, "token", buffer);
  NQJSONWriter_writeObjectEnd(&writer);
  NQJSONWriter_finalize(&writer);

  return NQ_HTTP_OK;
}

static int signupPostRequest(NQWebRequest* request, NQWebResponse* response)
{
  struct UserApiRequest* uas = (struct UserApiRequest*)request->userdata;

  NQWebResponse_setHeader(response, NQHTTP_HEADER_CONTENT_TYPE, NQ_MEDIATYPE_APPLICATION_JSON);

  struct UserSignup params;
  UserSignup_init(&params);

  if (!UserSignup_parse(&params, NQStringPrint_characters(&uas->recvBuffer), NQStringPrint_length(&uas->recvBuffer), true)) {
    NQWebResponse_writeJsonRpcErrorParams(response, NQ_HTTP_BAD_REQUEST, "Invalid params");
    UserSignup_finalize(&params);
    return NQ_HTTP_BAD_REQUEST;
  }

  if (!NQUserDataStoreSignup(uas->database, params.email, params.password)) {
    NQWebResponse_writeJsonRpcErrorParams(response, NQ_HTTP_INTERNAL_SERVER_ERROR, "Unable to register user");
    UserSignup_finalize(&params);
    return NQ_HTTP_INTERNAL_SERVER_ERROR;
  }

  int ret = commonPostResponse(uas, params.email, response);
  UserSignup_finalize(&params);
  return ret;
}

static int loginPostRequest(NQWebRequest* request, NQWebResponse* response)
{
  struct UserApiRequest* uas = (struct UserApiRequest*)request->userdata;

  NQWebResponse_setHeader(response, NQHTTP_HEADER_CONTENT_TYPE, NQ_MEDIATYPE_APPLICATION_JSON);

  struct UserSignup params;
  UserSignup_init(&params);

  if (!UserSignup_parse(&params, NQStringPrint_characters(&uas->recvBuffer), NQStringPrint_length(&uas->recvBuffer), false)) {
    NQWebResponse_writeJsonRpcErrorParams(response, NQ_HTTP_BAD_REQUEST, "Invalid params");
    UserSignup_finalize(&params);
    return NQ_HTTP_BAD_REQUEST;
  }

  if (!NQUserDataStoreLogin(uas->database, params.email, params.password)) {
    NQWebResponse_writeJsonRpcErrorParams(response, NQ_HTTP_INTERNAL_SERVER_ERROR, "Unable to register user");
    UserSignup_finalize(&params);
    return NQ_HTTP_INTERNAL_SERVER_ERROR;
  }

  int ret = commonPostResponse(uas, params.email, response);
  UserSignup_finalize(&params);
  return ret;
}

static void commonPostRelease(NQWebRequest* request)
{
  struct UserApiRequest* uas = (struct UserApiRequest*)request->userdata;
  NQStringPrint_finalize(&uas->recvBuffer);
  NQFree(uas);
}

static const NQWebRequestOperations kSignupOps = {
  .init    = commonInit,
  .receive = commonPostReceive,
  .handler = signupPostRequest,
  .release = commonPostRelease,
};

static const NQWebRequestOperations kLoginOps = {
  .init    = commonInit,
  .receive = commonPostReceive,
  .handler = loginPostRequest,
  .release = commonPostRelease,
};

static int restApiInit(NQWebExecutor* restApi, void* data)
{
  struct NQWebRestUsersParams* params = (struct NQWebRestUsersParams*)data;
  if (params->databasePath == NULL)
    return -NQ_EINVAL;

  struct NQWebRestUsersApi* usersApi = NQ_CONTAINER_OF(restApi, struct NQWebRestUsersApi, executor);
  usersApi->database = NQSQLiteDatabase_open(params->databasePath, kNQSQLiteOpenCreateReadWrite);
  if (usersApi->database == NULL) {
    return -NQ_EIO;
  }

  if (!NQUserDataStoreInit(usersApi->database)) {
    NQSQLiteDatabase_release(usersApi->database);
    return -NQ_EIO;
  }

  int ret = NQWebExecutor_addRequestListener(&usersApi->executor, &usersApi->signupListener, &kSignupOps, usersApi, NQ_HTTP_POST, params->signupUrl);
  if (ret) {
    NQUserDataStoreExit(usersApi->database);
    NQSQLiteDatabase_release(usersApi->database);
    return ret;
  }

  ret = NQWebExecutor_addRequestListener(&usersApi->executor, &usersApi->loginListener, &kLoginOps, usersApi, NQ_HTTP_POST, params->loginUrl);
  if (ret) {
    NQWebExecutor_removeRequestListener(&usersApi->executor, &usersApi->signupListener);
    NQUserDataStoreExit(usersApi->database);
    NQSQLiteDatabase_release(usersApi->database);
    return ret;
  }

  return 0;
}

static void restApiRelease(NQWebExecutor* restApi)
{
  struct NQWebRestUsersApi* usersApi = NQ_CONTAINER_OF(restApi, struct NQWebRestUsersApi, executor);

  NQWebExecutor_removeRequestListener(&usersApi->executor, &usersApi->loginListener);
  NQWebExecutor_removeRequestListener(&usersApi->executor, &usersApi->signupListener);

  NQUserDataStoreExit(usersApi->database);
  NQSQLiteDatabase_release(usersApi->database);
}

static const struct NQWebExecutorOperations kWebRestUsersOps = {
  .init = restApiInit,
  .release = restApiRelease,
};

NQWebRestUsersApi* NQWebRestUsersApiCreate(NQWebServer* server, const struct NQWebRestUsersParams* params)
{
  return (NQWebRestUsersApi*)NQWebServer_createExecutor(server, sizeof(struct NQWebRestUsersApi), &kWebRestUsersOps, (void*)params);
}

void NQWebRestUsersApiDestroy(NQWebServer* server, NQWebRestUsersApi* usersApi)
{
  NQWebServer_destroyExecutor(server, &usersApi->executor);
}
