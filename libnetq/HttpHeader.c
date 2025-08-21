/*
 * MIT License
 *
 * Copyright (c) 2022-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/HttpHeader.h"

#include <string.h>

static const char* s_strings[] = {
  NQHTTP_HEADER_ACCEPT_ENCODING,
  NQHTTP_HEADER_USER_AGENT,
  NQHTTP_HEADER_SERVER,
  NQHTTP_HEADER_AUTHORIZATION,
  NQHTTP_HEADER_CACHE_CONTROL,
  NQHTTP_HEADER_CONTENT_TYPE,
  NQHTTP_HEADER_CONTENT_DISPOSITION,
  NQHTTP_HEADER_HOST,
  NQHTTP_HEADER_UPGRADE,
  NQHTTP_HEADER_CONNECTION,
  NQHTTP_HEADER_ORIGIN,
  NQHTTP_HEADER_SEC_WEBSOCKET_ACCEPT,
  NQHTTP_HEADER_SEC_WEBSOCKET_KEY,
  NQHTTP_HEADER_SEC_WEBSOCKET_VERSION,
  NQHTTP_HEADER_SEC_WEBSOCKET_PROTOCOL,
  NQHTTP_HEADER_SEC_WEBSOCKET_EXTENSIONS,
};

const char* NQHTTPHeaderTypeToString(NQHTTPHeaderType type)
{
  return s_strings[type];
}

bool NQHTTPHeaderParse(const char* data, size_t size, NQHTTPHeader* result)
{
  const char* end = data + size;

  for (;;) {
    if (end <= data) {
      return false;
    }
    if (*data != ' ') {
      break;
    }
    data++;
  }

  size_t klen = 0;
  const char* key = data;
  for (;;) {
    if (end <= data) {
      return false;
    }
    char ch = *data++;
    if (ch == '=') {
      klen = data - key - 1;
      break;
    }
    if (ch != ' ') {
      continue;
    }
    klen = data - key - 1;
    for (;;) {
      if (end <= data) {
        return false;
      }
      ch = *data++;
      if (ch == '=') {
        break;
      }
      if (ch != ' ') {
        return false;
      }
    }
    break;
  }

  for (;;) {
    if (end <= data) {
      return false;
    }
    if (*data != ' ') {
      break;
    }
    data++;
  }

  if (*data != '"') {
    return false;
  }

  if (end <= ++data) {
    return false;
  }

  size_t vlen = 0;
  const char* val = data;
  for (;;) {
    if (end <= data) {
      return false;
    }
    if (data[vlen] == '"') {
      data += vlen + 1;
      break;
    }
    vlen++;
  }

  while (data < end) {
    if (*data != ' ') {
      return false;
    }
  }

  if (result != NULL) {
    result->keyData = key;
    result->keySize = klen;
    result->valData = val;
    result->valSize = vlen;
  }

  return true;
}

bool NQHttpFormDataParse(const char* data, size_t size, NQHttpFormData* result)
{
  size_t count = 0;
  const char* curr = data;
  const char* end = curr + size;

  if (result != NULL) {
    memset(result, 0, sizeof(*result));
  }

  while (curr < end) {
    while (curr < end && *curr == ' ') {
      curr++;
    }
    const char* data = curr;
    while (curr < end) {
      if (*curr++ == ';')
        break;
    }
    size_t size = curr - data - 1;
    while (size != 0 && data[size - 1] == ' ') {
      size--;
    }
    if (count == 0) {
      if (size != 9 || memcmp(data, "form-data", 9) != 0) {
        return false;
      }
      count++;
      continue;
    }
    NQHTTPHeader kv;
    if (NQHTTPHeaderParse(data, size, &kv)) {
      if (kv.keySize == 4 && memcmp("name", kv.keyData, kv.keySize) == 0) {
        if(result != NULL) {
          result->nameData = kv.valData;
          result->nameSize = kv.valSize;
        }
      }
      else if (kv.keySize == 8 && memcmp("filename", kv.keyData, kv.keySize) == 0) {
        if(result != NULL) {
          result->filenameData = kv.valData;
          result->filenameSize = kv.valSize;
        }
      }
    }
    count++;
    data += size;
  }

  return count > 0;
}

const char* NQHttpGetContentBoundary(const char* contentType)
{
  while (*contentType == ' ')
    contentType++;

  const char multipartFormData[] = "multipart/form-data;";
  if (memcmp(contentType, multipartFormData, sizeof(multipartFormData) - 1) != 0)
    return NULL;
  contentType += sizeof(multipartFormData) - 1;

  while (*contentType == ' ')
    contentType++;

  const char boundary[] = "boundary=";
  if (memcmp(contentType, boundary, sizeof(boundary) - 1) != 0)
    return NULL;
  contentType += sizeof(boundary) - 1;

  return contentType;
}
