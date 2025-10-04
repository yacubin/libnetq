/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQURL"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/URL.h"

#include <libnetq/CStrBase.h>
#include <libnetq/ObjectClass.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/Math.h>
#include <libnetq/Log.h>

extern const NQObjectClass __NQURLClass;

struct NQURLData {
  unsigned schemePosition;
  unsigned schemeLength;
  unsigned hostPosition;
  unsigned hostLength;
  unsigned portPosition;
  unsigned portLength;
  unsigned pathPosition;
  unsigned pathLength;
  unsigned queryPosition;
  unsigned queryLength;
  unsigned fragmentPosition;
  unsigned fragmentLength;
};

struct NQURL {
  const NQObjectClass* class;
  struct NQURLData data;
  unsigned length;
  char characters[1];
};

#define SCHEME_INDEX 0
#define HOST_INDEX 1
#define PORT_INDEX 2
#define PATH_INDEX 3
#define QUERY_INDEX 4
#define FRAGMENT_INDEX 5

static bool NQURLDataParse(const char* characters, size_t length, struct NQURLData* result)
{
  unsigned positions[6];
  unsigned lengths[6];
  char delimiters[6] = { 0, 0, ':', '/', '?', '#' };

  const char* start = strstr(characters, NQ_URL_SCHEME_SEPARATOR);
  if (start == NULL || start == characters) {
    NQ_LOGE("URL cannot be missing scheme");
    return false;
  }

  positions[SCHEME_INDEX] = 0;
  lengths[SCHEME_INDEX] = (unsigned)(start - characters);

  start = start + 3;

  positions[HOST_INDEX] = (unsigned)(start - characters);

  size_t curr = HOST_INDEX;
  for (size_t i = curr + 1; i < 6; i++) {
    const char* end = strchr(start, delimiters[i]);
    if (end == NULL) {
      positions[i] = 0;
      lengths[i] = 0;
      continue;
    }
    lengths[curr] = (unsigned)(end - start);
    start = end;
    positions[i] = (unsigned)(start - characters);
    curr = i;
  }

  lengths[curr] = length - (start - characters);
  if (lengths[HOST_INDEX] == 0) {
    NQ_LOGE("URL cannot be missing host");
    return false;
  }
  if (lengths[PORT_INDEX]) {
    if (lengths[PORT_INDEX] == 1) {
      NQ_LOGE("URL cannot be missing host");
      return false;
    }
    positions[PORT_INDEX]++;
    lengths[PORT_INDEX]--;
  }

  result->schemePosition = positions[SCHEME_INDEX];
  result->schemeLength = lengths[SCHEME_INDEX];
  result->hostPosition = positions[HOST_INDEX];
  result->hostLength = lengths[HOST_INDEX];
  result->portPosition = positions[PORT_INDEX];
  result->portLength = lengths[PORT_INDEX];
  result->pathPosition = positions[PATH_INDEX];
  result->pathLength = lengths[PATH_INDEX];
  result->queryPosition = positions[QUERY_INDEX];
  result->queryLength = lengths[QUERY_INDEX];
  result->fragmentPosition = positions[FRAGMENT_INDEX];
  result->fragmentLength = lengths[FRAGMENT_INDEX];

  return true;
}

NQURL* NQURL_create(const char* characters)
{
  size_t length = strlen(characters);
  if (length > NQ_UINT32_MAX) {
    NQ_LOGE("The URL string is too long");
    return NULL;
  }

  struct NQURLData data;
  if (!NQURLDataParse(characters, length, &data)) {
    return NULL;
  }

  NQURL* thiz = (NQURL*)NQMalloc(sizeof(NQURL) + length);
  if (thiz == NULL) {
    NQ_LOGE("No memory");
    return NULL;
  }

  thiz->class = &__NQURLClass;
  thiz->data = data;
  thiz->length = (unsigned)length;
  memcpy(thiz->characters, characters, length + 1);

  return thiz;
}

void NQURL_destroy(NQURL* thiz)
{
  NQFree(thiz);
}

const char* NQURL_characters(const NQURL* thiz)
{
  return thiz->characters;
}

size_t NQURL_lenght(NQURL* thiz)
{
  return thiz->length;
}

bool NQURL_isEmpty(const NQURL* thiz)
{
  return thiz->length == 0;
}

static inline size_t getComponent(const NQURL* thiz, unsigned position, unsigned length, char* buf, size_t len)
{
  size_t n = NQGetMin(length, len);
  memcpy(buf, thiz->characters + position, n);
  if (n < len)
    buf[n] = '\0';
  return length;
}

size_t NQURL_getScheme(const NQURL* thiz, char* buffer, size_t length)
{
  return getComponent(thiz, thiz->data.schemePosition, thiz->data.schemeLength, buffer, length);
}

size_t NQURL_getHost(const NQURL* thiz, char* buffer, size_t length)
{
  return getComponent(thiz, thiz->data.hostPosition, thiz->data.hostLength, buffer, length);
}

size_t NQURL_getPort(const NQURL* thiz, char* buffer, size_t length)
{
  return getComponent(thiz, thiz->data.portPosition, thiz->data.portLength, buffer, length);
}

size_t NQURL_getPath(const NQURL* thiz, char* buffer, size_t length)
{
  return getComponent(thiz, thiz->data.pathPosition, thiz->data.pathLength, buffer, length);
}

size_t NQURL_getQuery(const NQURL* thiz, char* buffer, size_t length)
{
  return getComponent(thiz, thiz->data.queryPosition, thiz->data.queryLength, buffer, length);
}

size_t NQURL_getFragment(const NQURL* thiz, char* buffer, size_t length)
{
  return getComponent(thiz, thiz->data.fragmentPosition, thiz->data.fragmentLength, buffer, length);
}

const NQObjectClass __NQURLClass = {
  NQURLObjectType,
  NQ_CLASS_NAME,
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQURL_destroy,
};
