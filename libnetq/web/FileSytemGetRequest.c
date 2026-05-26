/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/web/FileSytemGetRequest.h"

#include <libnetq/HttpHeader.h>
#include <libnetq/HttpStatus.h>
#include <libnetq/MediaType.h>
#include <libnetq/FileHandle.h>
#include <libnetq/fs/Stat.h>
#include <libnetq/Dir.h>
#include <libnetq/Path.h>
#include <libnetq/Log.h>
#include <libnetq/Assert.h>
#include <libnetq/web/WebServer.h>

#define SIZE_EMPTY_STR "      -"
#define DATETIME_EMPTY_STR "ACCESS_DENIED      "

#define KB NQ_UINT64_C(1024)
#define MB (KB * 1024)
#define GB (MB * 1024)
#define TB (GB * 1024)

static void printFileSize(char* buf, size_t len, uint64_t fileSize)
{
  char* ptr = buf + len - 1;
  if (ptr >= buf)
    *ptr-- = '\0';
  if (ptr >= buf) {
    if (fileSize < KB)
      /* */;
    else if (fileSize < MB) {
      fileSize /= KB;
      *ptr-- = 'K';
    }
    else if (fileSize < GB) {
      fileSize /= MB;
      *ptr-- = 'M';
    }
    else if (fileSize < TB) {
      fileSize /= MB;
      *ptr-- = 'G';
    }
    else {
      fileSize /= TB;
      *ptr-- = 'T';
    }
  }
  while (ptr >= buf) {
    if (fileSize == 0)
      *ptr-- = ' ';
    else {
      *ptr-- = '0' + (fileSize % 10);
      fileSize /= 10;
    }
  }
}

int NQFileSystemGetRequest(const char* urlPrefix, NQWebRequest* request, NQWebResponse* response)
{
  NQWebServer* server = NQWebRequest_server(request);

  const char* url = NQWebRequest_url(request);
  size_t urlLength = NQStrlen(url);

  size_t urlPrefixLength = strlen(urlPrefix);
  if (urlLength < urlPrefixLength || memcmp(url, urlPrefix, urlPrefixLength) != 0 || strstr(url, "//") != NULL) {
    NQWebResponse_setHeader(response, NQHTTP_HEADER_LOCATION, urlPrefix);
    return NQ_HTTP_MOVED_PERMANENTLY;
  }

  const char* pathSuffix = url + urlPrefixLength;
  const char* baseDir = (const char*)request->userdata;

  NQStringPrint pathBuf;
  NQStringPrint_init(&pathBuf);
  if (NQStringPrint_printf(&pathBuf, "%s/%s", baseDir, pathSuffix) < 0) {
    NQStringPrint_finalize(&pathBuf);
    return NQ_HTTP_INTERNAL_SERVER_ERROR;
  }

  char* path = NQStringPrint_characters(&pathBuf);
  size_t pathLen = NQStringPrint_length(&pathBuf);

  NQStat stat;
  if (path[pathLen - 1] != NQ_PATH_DELIMITER) {
    int err = NQGetStat(path, &stat);
    if (err) {
      NQ_LOGE("NQGetStat error %i", err);
      NQStringPrint_finalize(&pathBuf);
      return NQ_HTTP_INTERNAL_SERVER_ERROR;
    }

    if (NQStat_isFile(&stat)) {
      const char* contentType = NQWebServer_getMimeType(server, path);
      NQFileHandle handle = NQFileOpen(path, NQ_FOPEN_READ);
      if (NQIsFileInvalid(handle)) {
        NQStringPrint_finalize(&pathBuf);
        return NQ_HTTP_INTERNAL_SERVER_ERROR;
      }

      NQWebResponse_setHeader(response, NQHTTP_HEADER_CONTENT_TYPE, contentType);

      uint8_t buffer[1024];
      for (;;) {
        int len = NQFileRead(handle, buffer, sizeof(buffer));
        if (len < 0) {
          NQFileClose(handle);
          return NQ_HTTP_INTERNAL_SERVER_ERROR;
        }
        if (len == 0)
          break;
        NQWebResponse_write(response, (char*)buffer, (size_t)len);
      }

      NQFileClose(handle);
      return NQ_HTTP_OK;
    }

    NQStringPrint_finalize(&pathBuf);

    if (!NQStat_isDirectory(&stat))
      return NQ_HTTP_NOT_FOUND;

    NQStringPrint_init(&pathBuf);
    if (NQStringPrint_printf(&pathBuf, "%s/", url) < 0) {
      NQStringPrint_finalize(&pathBuf);
      return NQ_HTTP_INTERNAL_SERVER_ERROR;
    }

    const char* location = NQPathBuilder_characters(&pathBuf);
    NQWebResponse_setHeader(response, NQHTTP_HEADER_LOCATION, location);
    NQStringPrint_finalize(&pathBuf);
    return NQ_HTTP_MOVED_PERMANENTLY;
  }

  NQDir* dir =  NQDir_open(path);

  if (dir == NULL) {
    NQStringPrint_finalize(&pathBuf);
    return NQ_HTTP_NOT_FOUND;
  }

  NQWebResponse_setHeader(response, NQHTTP_HEADER_CONTENT_TYPE, NQ_MEDIATYPE_TEXT_HTML);
  NQWebResponse_printf(response, "<!DOCTYPE html>");
  NQWebResponse_printf(response, "<html>");
  NQWebResponse_printf(response, "<head>");
  NQWebResponse_printf(response, "<title>Index of %s</title>", url);
  NQWebResponse_printf(response, "</head>");
  NQWebResponse_printf(response, "<body>");
  NQWebResponse_printf(response, "<h1>Index of %s</h1>", url);
  NQWebResponse_printf(response, "<hr><pre>");

  if (strcmp(urlPrefix, url) != 0) {
    NQWebResponse_printf(response, "<a href=\"../\">..</a>\n");
  }

  static const char spaceStr[] = "                                                                               ";

  char sizeBuf[NQ_ARRAY_LENGTH(SIZE_EMPTY_STR)];
  char datetimeBuf[NQ_ARRAY_LENGTH(DATETIME_EMPTY_STR)];

  while (NQDir_next(dir)) {
    const char* name = NQDir_name(dir);
    if (name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0')))
      continue;
    size_t nlen = strlen(name);

    const char* delimeter = "";
    const char* sizeStr = SIZE_EMPTY_STR;
    const char* datetimeStr = DATETIME_EMPTY_STR;

    if (NQStringPrint_writeAll(&pathBuf, name, nlen)) {
      if (NQGetStat(NQStringPrint_characters(&pathBuf), &stat) == 0) {
        printFileSize(sizeBuf, sizeof(sizeBuf), NQStat_size(&stat));
        int sz = NQTimeMsFormat(NQStat_modificationTimeMs(&stat), NQ_DT_RFC3339, datetimeBuf, sizeof(datetimeBuf));
        NQ_ASSERT(sz == NQ_CSTR_LENGTH(DATETIME_EMPTY_STR));
        sizeStr = sizeBuf;
        datetimeStr = datetimeBuf;
      }
      NQStringPrint_resize(&pathBuf, pathLen);
    }

    size_t spaceLen = sizeof(spaceStr) - nlen - sizeof(sizeBuf) - 1 - sizeof(datetimeBuf) - 1;
    if (NQDir_isDirectory(dir)) {
      delimeter = "/";
      spaceLen--;
    }

    NQWebResponse_printf(response, "<a href=\"%s%s\">%s%s</a>%*.s%s%s\n", name, delimeter, name, delimeter, (int)spaceLen, spaceStr, datetimeStr, sizeStr);
  }
  NQDir_close(dir);

  NQWebResponse_printf(response, "</pre><hr>");
  NQWebResponse_printf(response, "</body>");
  NQWebResponse_printf(response, "</html>");

  NQStringPrint_finalize(&pathBuf);
  return NQ_HTTP_OK;
}
