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

#include <libnetq/Module.h>
#include <libnetq/CType.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/String.h>
#include <libnetq/Log.h>
#include <libnetq/Path.h>
#include <libnetq/Network.h>
#include <libnetq/HttpHeader.h>
#include <libnetq/asset/FileSystemAsset.h>
#include <libnetq/URL.h>
#include <libnetq/UrlPath.h>
#include <libnetq/MediaType.h>
#include <libnetq/Assert.h>
#include <libnetq/web/WebRequest.h>
#include <libnetq/web/WebResponse.h>

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

enum ExecutorType {
  kRequestExecutorType,
  kSocketExecutorType,
};

struct WebExecutorEntry {
  char* pattern;
  char* method;
  NQListHead list;
  enum MatchType matchType;

  bool (*match)   (void* userdata, NQWebRequest* request, NQWebSocket* sock);
  void (*release) (void* userdata);
  void* userdata;
};

NQWebServer* NQWebServer_create(const NQWebServerParams* params, const struct NQWebServerOperations* operations)
{
  NQWebServer* thiz = (NQWebServer*)NQZeroMalloc(sizeof(NQWebServer));
  if (!NQWebServer_init(thiz, params, operations)) {
    NQWebServer_finalize(thiz);
    NQFree((void*)thiz);
    return NULL;
  }
  return thiz;
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

bool NQWebServer_init(NQWebServer* thiz, const NQWebServerParams* params, const struct NQWebServerOperations* operations)
{
  thiz->operations = operations;

  NQStringStorage_init(&thiz->version);
  NQStringStorage_setCharacters(&thiz->version, params->version);

  NQStringStorage_init(&thiz->email);
  NQStringStorage_setCharacters(&thiz->email, params->email);

  NQStringStorage_init(&thiz->workDir);
  NQStringStorage_setCharacters(&thiz->workDir, params->workDir);

  NQStringStorage_init(&thiz->resourceDir);
  NQStringStorage_setCharacters(&thiz->resourceDir, params->resourceDir);

  thiz->host = NQUrlHost_create(params->host);

  thiz->tlsEnabled = params->tlsEnabled;
  if (!NQUrlHost_hasPort(thiz->host)) {
     NQUrlHost_setPort(thiz->host, thiz->tlsEnabled ? NQ_DEFAULT_HTTPS_PORT : NQ_DEFAULT_HTTP_PORT);
  }

  thiz->tlsKeyString = NQIsCStrNullOrEmpty(params->tlsKey) ? NULL : loadFileAsString(params->workDir, params->tlsKey);
  thiz->tlsCertString = NQIsCStrNullOrEmpty(params->tlsCert) ? NULL : loadFileAsString(params->workDir, params->tlsCert);
  thiz->asset = NQIsCStrNullOrEmpty(params->resourceDir) ? NULL : NQFileSystemAssetCreate(params->resourceDir);

  NQListHead_init(&thiz->requestExecutors);
  NQListHead_init(&thiz->socketExecutors);
  NQListHead_init(&thiz->writerExecutors);
  thiz->statistics = NQHttpStatistics_create();
  NQPrimitiveStorage_init(&thiz->storage, NULL);

  thiz->looper = NULL;
  thiz->mimetypes = NULL;

  thiz->userdata = NULL;
  return thiz->operations->init(thiz);
}

static void WebExecutorList_finalize(NQListHead* executorList)
{
  NQListHead* iter = executorList->next;
  while (iter != executorList) {
    NQListHead* next = iter->next;
    struct WebExecutorEntry* entry = NQ_CONTAINER_OF(iter, struct WebExecutorEntry, list);
    if (entry->release)
      entry->release(entry->userdata);
    NQFree(entry);
    iter = next;
  }
}

void NQWebServer_finalize(NQWebServer* thiz)
{
  thiz->operations->release(thiz);
  NQNetworkLooper_release(thiz->looper);

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

  if (thiz->mimetypes)
    NQKeyVal_release(thiz->mimetypes);

  WebExecutorList_finalize(&thiz->socketExecutors);
  WebExecutorList_finalize(&thiz->requestExecutors);

  if (thiz->asset != NULL)
    NQAsset_destroy(thiz->asset);

  NQStringStorage_finalize(&thiz->email);
  NQStringStorage_finalize(&thiz->version);
  NQStringStorage_finalize(&thiz->workDir);
  NQStringStorage_finalize(&thiz->resourceDir);

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

static bool NQWebServer_initBase(NQListHead* head, NQWebRequest* request, NQWebSocket* sock)
{
  const char* url = NQWebRequest_url(request);
  const char* method = NQWebRequest_method(request);

  NQ_ASSERT(!request->urlPath);
  NQUrlPath* urlPath = NQUrlPath_create(url, NULL, false);
  if (urlPath == NULL)
    return false;

  NQListHead* iter;
  for (iter = head->next; iter != head; iter = iter->next) {
    struct WebExecutorEntry* entry = NQ_CONTAINER_OF(iter, struct WebExecutorEntry, list);
    if (strcmp(entry->method, method) != 0)
      continue;

    switch (entry->matchType) {
    case kMatchText:
      if (strcmp(entry->pattern, url) != 0)
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

    if (entry->match == NULL || entry->match(entry->userdata, request, sock)) {
      if (request->urlPath != urlPath)
        NQUrlPath_destroy(urlPath);
      return true;
    }

    if (request->urlPath != urlPath)
      NQUrlPath_destroy(request->urlPath);

    request->urlPath = NULL;
  }

  NQUrlPath_destroy(urlPath);
  return false;
}

bool NQWebServer_initRequest(NQWebServer* thiz, NQWebRequest* request)
{
  return NQWebServer_initBase(&thiz->requestExecutors, request, NULL);
}

bool NQWebServer_initSocket(NQWebServer* thiz, NQWebRequest* request, NQWebSocket* sock)
{
  return NQWebServer_initBase(&thiz->socketExecutors, request, sock);
}

static struct WebExecutorEntry* WebExecutorEntryCreate(const char* method, const char* url)
{
  struct WebExecutorEntry* entry;

  size_t mlenz = strlen(method) + 1;
  size_t ulenz = strlen(url) + 1;

  entry = (struct WebExecutorEntry*)NQZeroMalloc(sizeof(*entry) + mlenz + ulenz);
  if (entry == NULL)
    return NULL;

  entry->method = (char*)entry + sizeof(*entry);
  (void)memcpy(entry->method, method, mlenz);

  entry->pattern = entry->method + mlenz;
  (void)memcpy(entry->pattern, url, ulenz);

  if (!strcmp(url, "*:before"))
    entry->matchType = kMatchAnyBefore;
  else if (!strcmp(url, "*"))
    entry->matchType = kMatchAny;
  else if (strchr(url, '*') != NULL)
    entry->matchType = kMatchPattern;
  else if (NQIsUrlPathPattern(url))
    entry->matchType = kMatchSegments;
  else
    entry->matchType = kMatchText;

  entry->match = NULL;
  entry->userdata = NULL;

  return entry;
}

static void NQWebServer_addExecutorEntry(NQListHead* head, struct WebExecutorEntry* entry)
{
  NQListHead* iter = head->next;
  for (;;) {
    if (iter == head) {
      NQListHead_addBack(head, &entry->list);
      break;
    }
    struct WebExecutorEntry* it = NQ_CONTAINER_OF(iter, struct WebExecutorEntry, list);
    if (it->matchType > entry->matchType) {
      NQListHead_addBack(&it->list, &entry->list);
      break;
    }
    iter = iter->next;
  }
}

static bool requestMatch(void* userdata, NQWebRequest* request, NQWebSocket* sock)
{
  NQWebRequestExecutor* requestExecutor = (NQWebRequestExecutor*)userdata;
  return requestExecutor->match(requestExecutor, request);
}

static void requestRelease(void* userdata)
{
  NQWebRequestExecutor* requestExecutor = (NQWebRequestExecutor*)userdata;
  requestExecutor->release(requestExecutor);
}

bool NQWebServer_registerRequestExecutor(NQWebServer* thiz, const char* method, const char* url, struct NQWebRequestExecutor* executor)
{
  struct WebExecutorEntry* entry = WebExecutorEntryCreate(method, url);
  if (entry == NULL)
    return false;

  entry->match = executor->match ? requestMatch : NULL;
  entry->release = executor->release ? requestRelease : NULL;
  entry->userdata = executor;

  NQWebServer_addExecutorEntry(&thiz->requestExecutors, entry);
  NQWebServer_allowMetric(thiz, method, url);
  return true;
}

static int onBlobRequest(NQWebRequest* request, NQWebResponse* response)
{
  NQWebBlob* blob = (NQWebBlob*)request->userdata;

  if (blob->type) {
    NQWebResponse_setHeader(response, NQHTTP_HEADER_CONTENT_TYPE, blob->type);
  }

  (void)NQWebResponse_write(response, blob->data, blob->size);
  return NQ_HTTP_OK;
}

static bool blobMatch(void* userdata, NQWebRequest* request, NQWebSocket* sock)
{
  struct WebBlob* blob = (struct WebBlob*)userdata;
  request->userdata = blob;
  request->onRequest = onBlobRequest;
  return true;
}

bool NQWebServer_registerRequestBlob(NQWebServer* thiz, const char* method, const char* url, const NQWebBlob* blob)
{
  struct WebExecutorEntry* entry = WebExecutorEntryCreate(method, url);
  if (entry == NULL)
    return false;

  entry->match = blobMatch;
  entry->release = NULL;
  entry->userdata = (void*)blob;

  NQWebServer_addExecutorEntry(&thiz->requestExecutors, entry);
  NQWebServer_allowMetric(thiz, method, url);
  return true;
}

static bool socketMatch(void* userdata, NQWebRequest* request, NQWebSocket* sock)
{
  NQWebSocketExecutor* socketExecutor = (NQWebSocketExecutor*)userdata;
  return socketExecutor->match(socketExecutor, sock);
}

static void socketRelease(void* userdata)
{
  NQWebSocketExecutor* socketExecutor = (NQWebSocketExecutor*)userdata;
  socketExecutor->release(socketExecutor);
}

bool NQWebServer_registerSocketExecutor(NQWebServer* thiz, const char* method, const char* url, struct NQWebSocketExecutor* executor)
{
  struct WebExecutorEntry* entry = WebExecutorEntryCreate(method, url);
  if (entry == NULL)
    return false;

  entry->match = executor->match ? socketMatch : NULL;
  entry->release = executor->release ? socketRelease : NULL;
  entry->userdata = executor;

  NQWebServer_addExecutorEntry(&thiz->socketExecutors, entry);
  NQWebServer_allowMetric(thiz, method, url);
  return true;
}

bool NQWebServer_registerWriter(NQWebServer* thiz, const char* contentType, const struct NQWebWriterOperations* operations, void* userdata)
{
  size_t len = strlen(contentType);
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

void NQWebServer_setLooper(NQWebServer* thiz, NQNetworkLooper* looper)
{
  if (thiz->looper)
    NQNetworkLooper_release(thiz->looper);

  thiz->looper = NQNetworkLooper_retain(looper);
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
