/*
 * MIT License
 *
 * Copyright (c) 2023-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/web/WebServer.h"

#define NQ_LOG_TAG "NQWebServer"

#include <libnetq/string/StringUtil.h>
#include <libnetq/CType.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/Log.h>
#include <libnetq/Path.h>
#include <libnetq/Network.h>
#include <libnetq/HttpHeader.h>
#include <libnetq/asset/FileSystemAsset.h>
#include <libnetq/URL.h>
#include <libnetq/UrlPath.h>
#include <libnetq/MediaType.h>
#include <libnetq/Assert.h>
#include <libnetq/Math.h>
#include <libnetq/ErrorCode.h>
#include <libnetq/random/CryptoRandom.h>
#include <libnetq/web/WebRequest.h>
#include <libnetq/web/WebResponse.h>
#include <libnetq/web/WebSocket.h>

struct WebWriterEntry {
  const struct NQWebWriterOperations* operations;
  void* userdata;
  NQListHead list;
  char contentType[1];
};

enum MatchType {
  kMatchText,
  kMatchSegments,
  kMatchPattern,
  kMatchAnyBefore,
  kMatchAny,
};

static NQ_LISTHEAD_DEFINE(g_registredOpsList);

#if defined(WITH_MHD)
extern NQWebServerOperations kMHDServerOperations;
#endif

#if defined(WITH_CIVETWEB)
extern NQWebServerOperations kCivetWebServerOperations;
#endif

static NQWebServerOperations* g_builtinOpsList[] = {
#if defined(WITH_MHD)
  &kMHDServerOperations,
#endif

#if defined(WITH_CIVETWEB)
  &kCivetWebServerOperations,
#endif
};

static const NQWebServerOperations* defaultOperations(void)
{
  if (!NQListHead_isEmpty(&g_registredOpsList))
    return NQ_CONTAINER_OF(g_registredOpsList.next, struct NQWebServerOperations, list);
  if (NQ_ARRAY_LENGTH(g_builtinOpsList))
    return g_builtinOpsList[0];
  return NULL;
}

NQWebServer* NQWebServer_create(const NQWebServerParams* params)
{
  NQWebServer* thiz = (NQWebServer*)NQZalloc(sizeof(NQWebServer));
  if (NQWebServer_init(thiz, params, NULL))
    return thiz;
  NQWebServer_finalize(thiz);
  NQFree((void*)thiz);
  return NULL;
}

void NQWebServer_destroy(NQWebServer* thiz)
{
  NQWebServer_finalize(thiz);
  NQFree((void*)thiz);
}

static NQString* loadFileAsString(const char* workDir, const char* filename)
{
  NQPathBuilder pathBuilder;
  if (!NQPathBuilder_initResolve2(&pathBuilder, workDir, filename)) {
    NQ_LOGE("Unable to resolve path %s", filename);
    return NULL;
  }

  NQString* result = NQString_fromFile(NQPathBuilder_characters(&pathBuilder));
  if (result == NULL) {
    NQ_LOGE("Unable to load %s", NQPathBuilder_characters(&pathBuilder));
  }

  NQPathBuilder_finalize(&pathBuilder);
  return result;
}

bool NQWebServer_init(NQWebServer* thiz, const NQWebServerParams* params, NQWebServerSupervisor* parent)
{
  thiz->parent = parent;
  if (parent && parent->serverOps)
    thiz->operations = parent->serverOps;
  else {
    thiz->operations = defaultOperations();
    if (thiz->operations == NULL) {
      NQ_LOGE("No server implementation available");
      return false;
    }
  }

  NQStringData_init(&thiz->email);
  NQStringData_set(&thiz->email, params->email);

  NQStringData_init(&thiz->workDir);
  NQStringData_set(&thiz->workDir, params->workDir);

  NQStringData_init(&thiz->resourceDir);
  NQStringData_set(&thiz->resourceDir, params->resourceDir);

  thiz->host = NQUrlHost_create(params->host);

  thiz->tlsEnabled = params->tlsEnabled;
  if (!NQUrlHost_hasPort(thiz->host)) {
     NQUrlHost_setPort(thiz->host, thiz->tlsEnabled ? NQ_DEFAULT_HTTPS_PORT : NQ_DEFAULT_HTTP_PORT);
  }

  thiz->tlsKeyString = NQCStrIsNullOrEmpty(params->tlsKey) ? NULL : loadFileAsString(params->workDir, params->tlsKey);
  thiz->tlsCertString = NQCStrIsNullOrEmpty(params->tlsCert) ? NULL : loadFileAsString(params->workDir, params->tlsCert);
  thiz->asset = NQCStrIsNullOrEmpty(params->resourceDir) ? NULL : NQFileSystemAssetCreate(params->resourceDir);

  NQListHead_init(&thiz->executors);
  NQListHead_init(&thiz->requestExecutors);
  NQListHead_init(&thiz->socketExecutors);
  NQListHead_init(&thiz->writerExecutors);
  NQListHead_init(&thiz->moduleList);
  thiz->statistics = NQHttpStatistics_create();
  NQPrimitiveStorage_init(&thiz->storage, thiz->parent ? thiz->parent->storage : NULL);

  NQGetCryptoRandom(thiz->sessionSeckey, sizeof(thiz->sessionSeckey));

  thiz->looper = thiz->parent ? thiz->parent->looper : NULL;
  thiz->mimetypes = NULL;

  thiz->userdata = NULL;
  return thiz->operations->init(thiz);
}

void NQWebExecutor_init(NQWebExecutor* thiz, const struct NQWebExecutorOperations* operations)
{
  thiz->server = NULL;
  thiz->userdata = NULL;
  thiz->operations = operations;
  NQListHead_init(&thiz->list);
}

NQWebExecutor* NQWebExecutor_alloc(size_t sizeInBytes, const struct NQWebExecutorOperations* mops)
{
  struct NQWebExecutor* thiz = (struct NQWebExecutor*)NQZalloc(NQGetMax(sizeof(*thiz), sizeInBytes));
  if (thiz == NULL)
    return NULL;

  NQWebExecutor_init(thiz, mops);
  return thiz;
}

void NQWebExecutor_release(NQWebExecutor* thiz)
{
  NQ_ASSERT(NQListHead_isEmpty(&thiz->list));
  NQFree(thiz);
}

static void moduleEntryRelease(NQWebServer* thiz, NQWebExecutor* entry)
{
  if (entry->operations && entry->operations->release)
    entry->operations->release(entry);
  if (entry->release)
    entry->release(entry);
  NQListHead_remove(&entry->list);
  NQWebExecutor_release(entry);
}

void NQWebServer_finalize(NQWebServer* thiz)
{
  thiz->operations->release(thiz);

  NQPrimitiveStorage_finalize(&thiz->storage);
  NQHttpStatistics_destroy(thiz->statistics);

  {
    NQListHead* iter = thiz->writerExecutors.next;
    while (iter != &thiz->writerExecutors) {
      struct WebWriterEntry* entry = NQ_CONTAINER_OF(iter, struct WebWriterEntry, list);
      NQListHead* next = entry->list.next;
      NQListHead_remove(&entry->list);
      NQFree(entry);
      iter = next;
    }
  }

  while (!NQListHead_isEmpty(&thiz->moduleList)) {
    NQWebExecutor* entry = NQ_CONTAINER_OF(thiz->moduleList.prev, struct NQWebExecutor, list);
    moduleEntryRelease(thiz, entry);
  }

  if (thiz->mimetypes)
    NQKeyVal_release(thiz->mimetypes);

  while (!NQListHead_isEmpty(&thiz->executors)) {
    NQWebExecutor* entry = NQ_CONTAINER_OF(thiz->executors.prev, struct NQWebExecutor, list);
    NQListHead_remove(&entry->list);
    NQFree(entry);
  }

  if (thiz->asset != NULL)
    NQAsset_destroy(thiz->asset);

  NQStringData_finalize(&thiz->email);
  NQStringData_finalize(&thiz->workDir);
  NQStringData_finalize(&thiz->resourceDir);

  if (thiz->tlsKeyString != NULL)
    NQString_release(thiz->tlsKeyString);

  if (thiz->tlsCertString != NULL)
    NQString_release(thiz->tlsCertString);

}

static bool comparePattern(const char* pattern, const char* url)
{
  char a = *pattern++;
  if (a == '\0')
    return false;

  for (;;) {
    char b = *url++;
    if (a == b) {
      if (a == '\0')
        return true;
      NQ_ASSERT(b != '*');
      a = *(pattern++);
      continue;
    }
    if (a != '*')
      return false;
    if (pattern[0] == '\0')
      return true;
    if (comparePattern(pattern, url))
      return true;
  }
}

bool NQWebServer_initRequest(NQWebServer* thiz, NQWebRequest* request)
{
  NQListHead* head = &thiz->requestExecutors;
  const char* url = NQWebRequest_url(request);
  const char* method = NQWebRequest_method(request);

  NQ_ASSERT(!request->urlPath);
  NQUrlPath* urlPath = NQUrlPath_create(url, NULL, false);
  if (urlPath == NULL)
    return false;

  NQListHead* iter;
  for (iter = head->next; iter != head; iter = iter->next) {
    struct NQWebRequestListener* entry = NQ_CONTAINER_OF(iter, struct NQWebRequestListener, list);

    if (NQStrcmp(entry->method, method) != 0)
      continue;

    switch (entry->patternKind) {
    case kMatchText:
      if (NQStrcmp(entry->pattern, url) != 0)
        continue;
      request->urlPath = urlPath;
      break;

    case kMatchSegments:
      request->urlPath = NQUrlPath_create(url, entry->pattern, true);
      if (request->urlPath == NULL)
        continue;
      break;

    case kMatchPattern:
      if (!comparePattern(entry->pattern, url))
        continue;
      request->urlPath = urlPath;
      break;

    case kMatchAnyBefore:
    case kMatchAny:
      request->urlPath = urlPath;
      break;

    default:
      continue;
    }

    request->operations = entry->operations;
    request->userdata = entry->userdata;
    if (request->operations->init == NULL || request->operations->init(request, entry->userdata) == 0) {
      if (request->urlPath != urlPath)
        NQUrlPath_destroy(urlPath);
      return true;
    }

    if (request->urlPath != urlPath)
      NQUrlPath_destroy(request->urlPath);

    request->operations = NULL;
    request->urlPath = NULL;
    request->userdata = NULL;
  }

  NQUrlPath_destroy(urlPath);
  return false;
}

bool NQWebServer_initSocket(NQWebServer* thiz, NQWebRequest* request, NQWebSocket* sock)
{
  NQListHead* head = &thiz->socketExecutors;
  const char* url = NQWebRequest_url(request);
  const char* method = NQWebRequest_method(request);

  NQ_ASSERT(!request->urlPath);
  NQUrlPath* urlPath = NQUrlPath_create(url, NULL, false);
  if (urlPath == NULL)
    return false;

  NQListHead* iter;
  for (iter = head->next; iter != head; iter = iter->next) {
    struct NQWebSocketListener* entry = NQ_CONTAINER_OF(iter, struct NQWebSocketListener, list);

    if (NQStrcmp(entry->method, method) != 0)
      continue;

    switch (entry->patternKind) {
    case kMatchText:
      if (NQStrcmp(entry->pattern, url) != 0)
        continue;
      request->urlPath = urlPath;
      break;

    case kMatchSegments:
      request->urlPath = NQUrlPath_create(url, entry->pattern, true);
      if (request->urlPath == NULL)
        continue;
      break;

    case kMatchPattern:
      if (!comparePattern(entry->pattern, url))
        continue;
      request->urlPath = urlPath;
      break;

    case kMatchAnyBefore:
    case kMatchAny:
      request->urlPath = urlPath;
      break;

    default:
      continue;
    }

    sock->operations = entry->operations;
    sock->userdata = entry->userdata;
    if (sock->operations->init == NULL || sock->operations->init(sock, entry->userdata) == 0) {
      if (request->urlPath != urlPath)
        NQUrlPath_destroy(urlPath);
      return true;
    }

    if (request->urlPath != urlPath)
      NQUrlPath_destroy(request->urlPath);

    request->operations = NULL;
    request->urlPath = NULL;
    request->userdata = NULL;
  }

  NQUrlPath_destroy(urlPath);
  return false;
}

static int getPatternKind(const char* pattern)
{
  if (!NQStrcmp(pattern, "*:before"))
    return kMatchAnyBefore;
  else if (!NQStrcmp(pattern, "*"))
    return kMatchAny;
  else if (strchr(pattern, '*') != NULL)
    return kMatchPattern;
  else if (NQIsUrlPathPattern(pattern))
    return kMatchSegments;
  else
    return kMatchText;
}

static int addRequestListener(NQWebServer* thiz, struct NQWebRequestListener* entry)
{
  if (NQCStrIsNullOrEmpty(entry->method) || NQCStrIsNullOrEmpty(entry->pattern))
    return -NQ_EINVAL;

  entry->patternKind = getPatternKind(entry->pattern);

  NQListHead* iter = thiz->requestExecutors.next;
  for (;;) {
    if (iter == &thiz->requestExecutors) {
      NQListHead_addBack(&thiz->requestExecutors, &entry->list);
      break;
    }
    struct NQWebRequestListener* it = NQ_CONTAINER_OF(iter, struct NQWebRequestListener, list);
    if (it->patternKind > entry->patternKind) {
      NQListHead_addBack(&it->list, &entry->list);
      break;
    }
    iter = iter->next;
  }

  if (entry->patternKind == kMatchText) {
    NQWebServer_allowMetric(thiz, entry->method, entry->pattern);
  }

  return 0;
}

static void removeRequestListener(NQWebServer* thiz, struct NQWebRequestListener* executor)
{
  NQ_ASSERT(!NQListHead_isEmpty(&executor->list));
  // if (entry->type == kMatchText)
  // NQWebServer_removeMetric
  NQListHead_remove(&executor->list);
}

int NQWebExecutor_addRequestListener(NQWebExecutor* executor, struct NQWebRequestListener* listener, const NQWebRequestOperations* operations, void* userdata, const char* method, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  char* newPattern = NQCStrFormatV(format, args);
  va_end(args);

  if (newPattern == NULL)
    return -NQ_ENOMEM;

  char* newMethod = NQCStrDuplicate(method);
  if (newMethod == NULL) {
    NQCStrFree(newMethod);
    return -NQ_ENOMEM;
  }

  listener->method = newMethod;
  listener->pattern = newPattern;
  listener->executor = executor;
  NQListHead_init(&listener->list);
  listener->userdata = userdata;
  listener->operations = operations;

  int ret = addRequestListener(executor->server, listener);
  if (ret) {
    NQCStrFree(newMethod);
    NQCStrFree(newPattern);
  }

  return ret;
}

void NQWebExecutor_removeRequestListener(NQWebExecutor* executor, struct NQWebRequestListener* listener)
{
  removeRequestListener(executor->server, listener);
  NQCStrFree(listener->method);
  NQCStrFree(listener->pattern);
}

static int addSocketListener(NQWebServer* thiz, struct NQWebSocketListener* entry)
{
  if (entry->method == NULL || entry->pattern == NULL)
    return -NQ_EINVAL;

  entry->patternKind = getPatternKind(entry->pattern);

  NQListHead* iter = thiz->socketExecutors.next;
  for (;;) {
    if (iter == &thiz->socketExecutors) {
      NQListHead_addBack(&thiz->socketExecutors, &entry->list);
      break;
    }
    struct NQWebRequestListener* it = NQ_CONTAINER_OF(iter, struct NQWebRequestListener, list);
    if (it->patternKind > entry->patternKind) {
      NQListHead_addBack(&it->list, &entry->list);
      break;
    }
    iter = iter->next;
  }

  return 0;
}

static void removeSocketListener(NQWebServer* thiz, struct NQWebSocketListener* executor)
{
  NQListHead_remove(&executor->list);
}

int NQWebExecutor_addSocketListener(NQWebExecutor* executor, struct NQWebSocketListener* listener, const NQWebSocketOperations* operations, void* userdata, const char* method, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  char* newPattern = NQCStrFormatV(format, args);
  va_end(args);

  if (newPattern == NULL)
    return -NQ_ENOMEM;

  char* newMethod = NQCStrDuplicate(method);
  if (newMethod == NULL) {
    NQCStrFree(newMethod);
    return -NQ_ENOMEM;
  }

  listener->method = newMethod;
  listener->pattern = newPattern;
  listener->executor = executor;
  NQListHead_init(&listener->list);
  listener->userdata = userdata;
  listener->operations = operations;

  int ret = addSocketListener(executor->server, listener);
  if (ret) {
    NQCStrFree(newMethod);
    NQCStrFree(newPattern);
  }

  return ret;
}

void NQWebExecutor_removeSocketListener(NQWebExecutor* executor, struct NQWebSocketListener* listener)
{
  removeSocketListener(executor->server, listener);
  NQCStrFree(listener->method);
  NQCStrFree(listener->pattern);
}

struct NQWebRequestExecutor {
  NQWebExecutor executor;
  size_t listenerCount;
  struct NQWebRequestListener listeners[1];
};

void releaseRequestExecutor(NQWebExecutor* executor)
{
  struct NQWebRequestExecutor* execApi = NQ_CONTAINER_OF(executor, struct NQWebRequestExecutor, executor);
  for (size_t i = 0; i < execApi->listenerCount; i++)
    NQWebExecutor_removeRequestListener(&execApi->executor, &execApi->listeners[i]);
}

NQWebExecutor* NQWebServer_createRequestExecutor(NQWebServer* thiz, const char* method, const char* url, const NQWebRequestOperations* operations, void* data)
{
  const NQWebRequestMatch matches[] = { { .method = method, .url = url }, { NULL } };
  return NQWebServer_createRequestExecutorEx(thiz, matches, operations, data);
}

NQWebExecutor* NQWebServer_createRequestExecutorEx(NQWebServer* thiz, const NQWebRequestMatch* matches, const NQWebRequestOperations* operations, void* data)
{
  size_t listenerCount = 0;
  for (const NQWebRequestMatch* iter = matches; iter->method && iter->url; iter = &matches[listenerCount])
    listenerCount++;
  if (listenerCount == 0)
    return NULL;

  struct NQWebRequestExecutor* execApi;
  size_t sizeInBytes = sizeof(struct NQWebRequestExecutor) - sizeof(execApi->listeners) + sizeof(*execApi->listeners) * listenerCount;
  execApi = (struct NQWebRequestExecutor*)NQWebServer_createExecutor(thiz, sizeInBytes, NULL, NULL);
  if (execApi == NULL)
    return NULL;

  for (size_t index = 0; index < listenerCount; index++) {
    int ret = NQWebExecutor_addRequestListener(&execApi->executor, &execApi->listeners[index], operations, data, matches[index].method, "%s", matches[index].url);
    if (ret) {
      for (size_t i = 0; i < index; i++)
        NQWebExecutor_removeRequestListener(&execApi->executor, &execApi->listeners[i]);
      NQWebServer_destroyExecutor(thiz, &execApi->executor);
      return NULL;
    }
  }

  execApi->listenerCount = listenerCount;
  execApi->executor.release = releaseRequestExecutor;
  return &execApi->executor;
}

static int onBlobInit(NQWebRequest* request, void* data)
{
  request->userdata = data;
  return 0;
}

static int onBlobHandler(NQWebRequest* request, NQWebResponse* response)
{
  NQWebBlob* blob = (NQWebBlob*)request->userdata;

  if (blob->type) {
    NQWebResponse_setHeader(response, NQHTTP_HEADER_CONTENT_TYPE, blob->type);
  }

  (void)NQWebResponse_write(response, blob->data, blob->size);
  return NQ_HTTP_OK;
}

static const NQWebRequestOperations kBlobOps = {
  .init = onBlobInit,
  .handler = onBlobHandler,
};

NQWebExecutor* NQWebServer_createRequestBlob(NQWebServer* thiz, const char* method, const char* url, const NQWebBlob* blob)
{
  return NQWebServer_createRequestExecutor(thiz, method, url, &kBlobOps, (void*)blob);
}

NQWebExecutor* NQWebServer_createRequestBlobEx(NQWebServer* thiz, const NQWebRequestMatch* matches, const NQWebBlob* blob)
{
  return NQWebServer_createRequestExecutorEx(thiz, matches, &kBlobOps, (void*)blob);
}

struct NQWebSocketExecutor {
  NQWebExecutor executor;
  size_t listenerCount;
  struct NQWebSocketListener listeners[1];
};

void releaseSocketExecutor(NQWebExecutor* pexec)
{
  struct NQWebSocketExecutor* execApi = NQ_CONTAINER_OF(pexec, struct NQWebSocketExecutor, executor);
  for (size_t i = 0; i < execApi->listenerCount; i++)
    NQWebExecutor_removeSocketListener(&execApi->executor, &execApi->listeners[i]);
}

NQWebExecutor* NQWebServer_createSocketExecutor(NQWebServer* thiz, const char* method, const char* url, const NQWebSocketOperations* operations, void* data)
{
  const NQWebRequestMatch matches[] = { { .method = method, .url = url }, { NULL } };
  return NQWebServer_createSocketExecutorEx(thiz, matches, operations, data);
}

NQWebExecutor* NQWebServer_createSocketExecutorEx(NQWebServer* thiz, const NQWebRequestMatch* matches, const NQWebSocketOperations* operations, void* data)
{
  size_t listenerCount = 0;
  for (const NQWebRequestMatch* iter = matches; iter->method && iter->url; iter = &matches[listenerCount])
    listenerCount++;
  if (listenerCount == 0)
    return NULL;

  struct NQWebSocketExecutor* execApi;
  size_t sizeInBytes = sizeof(struct NQWebSocketExecutor) - sizeof(execApi->listeners) + sizeof(*execApi->listeners) * listenerCount;
  execApi = (struct NQWebSocketExecutor*)NQWebServer_createExecutor(thiz, sizeInBytes, NULL, NULL);
  if (execApi == NULL)
    return NULL;

  for (size_t index = 0; index < listenerCount; index++) {
    int ret = NQWebExecutor_addSocketListener(&execApi->executor, &execApi->listeners[index], operations, data, matches[index].method, "%s", matches[index].url);
    if (ret) {
      for (size_t i = 0; i < index; i++)
        NQWebExecutor_removeSocketListener(&execApi->executor, &execApi->listeners[i]);
      NQWebServer_destroyExecutor(thiz, &execApi->executor);
      return NULL;
    }
  }

  execApi->listenerCount = listenerCount;
  execApi->executor.release = releaseRequestExecutor;
  return &execApi->executor;
}

bool NQWebServer_registerWriter(NQWebServer* thiz, const char* contentType, const struct NQWebWriterOperations* operations, void* userdata)
{
  size_t len = NQStrlen(contentType);
  struct WebWriterEntry* entry = (struct WebWriterEntry*)NQMalloc(sizeof(*entry) + len);
  if (entry == NULL)
    return false;

  entry->operations = operations;
  entry->userdata = userdata;
  memcpy(entry->contentType, contentType, len + 1);
  NQListHead_addBack(&thiz->writerExecutors, &entry->list);

  return true;
}

void NQWebServer_unregisterWriter(NQWebServer* thiz, const struct NQWebWriterOperations* operations, void* userdata)
{
  NQListHead* iter = thiz->writerExecutors.next;
  while (iter != &thiz->writerExecutors) {
    struct WebWriterEntry* entry = NQ_CONTAINER_OF(iter, struct WebWriterEntry, list);
    if (entry->operations == operations) {
      NQListHead_remove(&entry->list);
      NQFree(entry);
      break;
    }
    iter = iter->next;
  }
}

struct NQWebWriter* NQWebServer_createWriterChain(NQWebServer* thiz, const char* contentType, NQWebRequest* request)
{
  struct NQWebWriter* result = NULL;
  NQListHead* iter = thiz->writerExecutors.prev;
  while (iter != &thiz->writerExecutors) {
    struct WebWriterEntry* entry = NQ_CONTAINER_OF(iter, struct WebWriterEntry, list);
    if (!strcmp(entry->contentType, contentType)) {
      struct NQWebWriter* writer = (struct NQWebWriter*)NQMalloc(sizeof(*writer));
      if (writer == NULL) {
        break;
      }

      writer->operations = entry->operations;
      writer->userdata = entry->userdata;
      writer->request = request;
      writer->next = NULL;

      if (writer->operations->init == NULL || writer->operations->init(writer) == 0) {
        writer->next = result;
        result = writer;
      }
      else {
        NQFree(writer);
      }
    }
    iter = iter->prev;
  }

  return result;
}

bool NQWebServer_allowMetric(NQWebServer* thiz, const char* method, const char* url)
{
  return NQIsUrlPath(url) && NQHttpStatistics_add(thiz->statistics, method, url);
}

NQUint8Array* NQWebServer_loadAssetBytes(const NQWebServer* thiz, const char* filename)
{
  if (thiz->asset == NULL) {
    return NULL;
  }

  return NQAssetFile_loadBytes(thiz->asset, filename);
}

const char* NQWebServer_getMimeType(const NQWebServer* thiz, const char* filename)
{
  if (thiz->mimetypes == NULL)
    return NQ_MEDIATYPE_APPLICATION_OCTETSTREAM;

  const char* extension = strrchr(filename, '.');
  if (extension == NULL)
    return NQ_MEDIATYPE_APPLICATION_OCTETSTREAM;

  const char* contentType = NQKeyVal_get(thiz->mimetypes, extension);
  if (contentType == NULL)
    return NQ_MEDIATYPE_APPLICATION_OCTETSTREAM;

  return contentType;
}

void NQWebServer_setMimeTypes(NQWebServer* thiz, NQKeyVal* mimetypes)
{
  if (thiz->mimetypes)
    NQKeyVal_release(thiz->mimetypes);
  thiz->mimetypes = NQKeyVal_retain(mimetypes);
}

int NQWebServer_start(NQWebServer* thiz)
{
  return thiz->operations->start(thiz);
}

int NQWebServer_stop(NQWebServer* thiz)
{
  return thiz->operations->stop(thiz);
}

NQWebExecutor* NQWebServer_createExecutor(NQWebServer* thiz, size_t sizeInBytes, const struct NQWebExecutorOperations* operations, void* data)
{
  NQWebExecutor* executor = NQWebExecutor_alloc(sizeInBytes, operations);
  executor->server = thiz;
  executor->release = NULL;
  NQListHead_addBack(&thiz->moduleList, &executor->list);

  if (executor->operations && executor->operations->init) {
    int ret = executor->operations->init(executor, data);
    if (ret != 0) {
      NQListHead_remove(&executor->list);
      NQWebExecutor_release(executor);
      return NULL;
    }
  }

  return executor;
}

void NQWebServer_destroyExecutor(NQWebServer* thiz, NQWebExecutor* executor)
{
  NQ_ASSERT(thiz == executor->server);
  moduleEntryRelease(thiz, executor);
}

void NQWebServerOperationsRegister(NQWebServerOperations* operations)
{
  NQListHead_addBack(&g_registredOpsList, &operations->list);
}

void NQWebServerOperationsUnregister(NQWebServerOperations* operations)
{
  NQListHead_remove(&operations->list);
}
