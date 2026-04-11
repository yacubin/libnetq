/*
 * MIT License
 *
 * Copyright (c) 2023-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBSERVER_H
#define _LIBNETQ_WEB_WEBSERVER_H

#include <libnetq/Asset.h>
#include <libnetq/Array.h>
#include <libnetq/String.h>
#include <libnetq/HttpStatus.h>
#include <libnetq/HttpMethod.h>
#include <libnetq/PrimitiveStorage.h>
#include <libnetq/NetworkLooper.h>
#include <libnetq/web/HttpStatistics.h>
#include <libnetq/web/UrlHost.h>
#include <libnetq/json/JSON.h>
#include <libnetq/KeyVal.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebRequest NQWebRequest;
typedef struct NQWebResponse NQWebResponse;
typedef struct NQWebSocket NQWebSocket;

typedef struct NQWebServer NQWebServer;

typedef struct NQWebSocketOperations NQWebSocketOperations;
typedef struct NQWebWriterOperations NQWebWriterOperations;

typedef struct NQWebRequestExecutor NQWebRequestExecutor;
struct NQWebRequestExecutor {
  bool (*match)   (struct NQWebRequestExecutor*, NQWebRequest* request); // connection
  void (*release) (struct NQWebRequestExecutor*);
};

typedef struct NQWebSocketExecutor NQWebSocketExecutor;
struct NQWebSocketExecutor {
  bool (*match)   (struct NQWebSocketExecutor*, NQWebSocket* socket); // connection
  void (*release) (struct NQWebSocketExecutor*);
};

typedef struct NQWebServerParams NQWebServerParams;
struct NQWebServerParams {
  const char* version;
  const char* email;
  const char* host;

  bool tlsEnabled;
  const char* tlsKey;
  const char* tlsCert;

  const char* workDir;
  const char* resourceDir;
};

typedef struct NQWebBlob NQWebBlob;
struct NQWebBlob {
  const char* type;
  const void* data;
  uint32_t size;
};

typedef struct NQWebServerOperations NQWebServerOperations;
struct NQWebServerOperations {
  bool   (*init)     (NQWebServer*);
  int    (*start)    (NQWebServer*);
  int    (*stop)     (NQWebServer*);
  void   (*release)  (NQWebServer*);
};

struct NQWebServer {
  NQPrimitiveStorage storage;
  NQListHead requestExecutors;
  NQListHead socketExecutors;
  NQListHead writerExecutors;
  NQHttpStatistics* statistics;
  NQUrlHost* host;
  NQStringStorage version;
  NQStringStorage email;
  NQStringStorage workDir;
  NQStringStorage resourceDir;
  NQString* tlsKeyString;
  NQString* tlsCertString;
  bool tlsEnabled;
  NQNetworkLooper* looper;
  NQAsset* asset;
  NQKeyVal* mimetypes;

  const struct NQWebServerOperations* operations;
  void* userdata;
};

NQ_EXPORT NQWebServer* NQWebServer_create(const NQWebServerParams* params, const struct NQWebServerOperations* operations);
NQ_EXPORT bool NQWebServer_init(NQWebServer*, const NQWebServerParams* params, const struct NQWebServerOperations* operations);
NQ_EXPORT void NQWebServer_destroy(NQWebServer*);
NQ_EXPORT void NQWebServer_finalize(NQWebServer*);

NQ_EXPORT int NQWebServer_start(NQWebServer* service);
NQ_EXPORT int NQWebServer_stop(NQWebServer* service);

static inline const char* NQWebServer_version(const NQWebServer* thiz)
{
  return NQStringStorage_characters(&thiz->version);
}

static inline const char* NQWebServer_email(const NQWebServer* thiz)
{
  return NQStringStorage_characters(&thiz->email);
}

static inline const char* NQWebServer_workDir(const NQWebServer* thiz)
{
  return NQStringStorage_characters(&thiz->workDir);
}

static inline const char* NQWebServer_resourceDir(const NQWebServer* thiz)
{
  return NQStringStorage_characters(&thiz->resourceDir);
}

static inline bool NQWebServer_tlsEnabled(const NQWebServer* thiz)
{
  return thiz->tlsEnabled;
}

static inline char* NQWebServer_tlsKey(const NQWebServer* thiz)
{
  return NQString_characters(thiz->tlsKeyString);
}

static inline const char* NQWebServer_tlsCert(const NQWebServer* thiz)
{
  return NQString_characters(thiz->tlsCertString);
}

NQ_EXPORT bool NQWebServer_registerRequestBlob(NQWebServer*, const char* method, const char* url, const NQWebBlob* blob);
NQ_EXPORT bool NQWebServer_registerRequestExecutor(NQWebServer*, const char* method, const char* url, struct NQWebRequestExecutor* executor);
NQ_EXPORT bool NQWebServer_registerSocketExecutor(NQWebServer*, const char* method, const char* url, struct NQWebSocketExecutor* executor);
NQ_EXPORT struct NQWebWriter* NQWebServer_createWriterChain(NQWebServer*, const char* contentType, NQWebRequest* request);
NQ_EXPORT bool NQWebServer_registerWriter(NQWebServer*, const char* contentType, const struct NQWebWriterOperations* operations, void* userdata);
NQ_EXPORT void NQWebServer_unregisterWriter(NQWebServer*, const struct NQWebWriterOperations* operations, void* userdata);

NQ_EXPORT bool NQWebServer_allowMetric(NQWebServer*, const char* method, const char* url);

static inline NQHttpStatistics* NQWebServer_statistics(NQWebServer* thiz)
{
  return thiz->statistics;
}

static inline NQPrimitiveStorage* NQWebServer_storage(NQWebServer* thiz)
{
  return &thiz->storage;
}

static inline NQUrlHost* NQWebServer_host(NQWebServer* thiz)
{
  return thiz->host;
}

static inline NQNetworkLooper* NQWebServer_looper(NQWebServer* thiz)
{
  return thiz->looper;
}

NQ_EXPORT void NQWebServer_setLooper(NQWebServer*, NQNetworkLooper* looper);

static inline NQAsset* NQWebServer_asset(const NQWebServer* thiz)
{
  return thiz->asset;
}

NQ_EXPORT NQUint8Array* NQWebServer_loadAssetBytes(const NQWebServer*, const char* filename);
NQ_EXPORT const char* NQWebServer_getMimeType(const NQWebServer*, const char* filename);
NQ_EXPORT void NQWebServer_setMimeTypes(NQWebServer*, NQKeyVal* mimetypes);

NQ_EXPORT bool NQWebServer_initRequest(NQWebServer*, NQWebRequest* request);
NQ_EXPORT bool NQWebServer_initSocket(NQWebServer*, NQWebRequest* request, NQWebSocket* sock);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WEBSERVER_H */
