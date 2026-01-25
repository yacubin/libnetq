/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Path.h"

#include <libnetq/UTF.h>
#include <libnetq/CStrBase.h>
#include <libnetq/Math.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/Assert.h>

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#endif

NQ_STATIC_ASSERT(sizeof(uint16_t) == sizeof(NQWChar), "Bad size of NQWChar");

static inline char normalizeCharacter(char ch)
{
  return ch == NQ_WINPATH_DELIMITER ? NQ_PATH_DELIMITER : ch;
}

NQPath* NQPath_create(const char* path)
{
  size_t len = strlen(path);
  if (len > NQ_UINT16_MAX)
    return NULL;

  NQPath* thiz = (NQPath*)NQMalloc(sizeof(*thiz) + len);
  if (thiz == NULL)
    return NULL;

  thiz->length = len;
  char* ptr = thiz->characters;
  for (size_t i = 0; i < len; i++) {
    *ptr++ = normalizeCharacter(path[i]);
  }
  *ptr++ = '\0';

  return thiz;
}

NQPath* NQPath_fromJoin2(const char* path1, const char* path2)
{
  size_t len1 = strlen(path1);
  size_t len2 = strlen(path2);
  size_t length = len1 + 1 + len2;
  if (length > NQ_UINT16_MAX || length < len1 || length < len2)
    return NULL;

  NQPath* thiz = (NQPath*)NQMalloc(sizeof(*thiz) + length);
  if (thiz == NULL)
    return NULL;

  thiz->length = length;
  char* ptr = thiz->characters;
  for (size_t i = 0; i < len1; i++) {
    *ptr++ = normalizeCharacter(path1[i]);
  }

  *ptr++ = NQ_PATH_DELIMITER;

  for (size_t i = 0; i < len2; i++) {
    *ptr++ = normalizeCharacter(path2[i]);
  }

  *ptr++ = '\0';

  return thiz;
}

void NQPath_destroy(NQPath* thiz)
{
  NQFree(thiz);
}

size_t NQPathFrom(char* buffer, size_t n, const NQWChar* path)
{
  if (n == 0)
    return 0;

  NQUChar character;
  const uint16_t* utf16Start = (const uint16_t*)path;
  uint8_t* utf8Start = (uint8_t*)buffer;
  uint8_t* utf8End = utf8Start + n;

  while (*utf16Start != '\0') {
    character = NQUCharNext16(utf16Start, utf16Start + 2, &utf16Start);
    if (!NQIsUChar(character)) {
      NQ_ASSERT(0);
      *buffer = '\0';
      return 0; // TODO: return length
    }

    if (character == NQ_WINPATH_DELIMITER)
      character = NQ_PATH_DELIMITER;

    if (!NQUCharPush8(utf8Start, utf8End, &utf8Start, character)) {
      NQ_ASSERT(0);
      *buffer = '\0';
      return 0; // TODO: return length
    }
  }
  
  if (utf8Start < utf8End)
    *utf8Start = '\0';

  return utf8Start - (uint8_t*)buffer;
}

size_t NQGetAbsolutePath(char* buffer, size_t n, const char* path)
{
  // TODO
  // https://chromium.googlesource.com/native_client/nacl-newlib/+/a9ae3c60b36dea3d8a10e18b1b6db952d21268c2/newlib/libc/sys/linux/realpath.c
  //realpath(const char *path, char *resolved_path);
  //  NQIsAbsolutePath(path)
  size_t len = strlen(path);
  size_t sz = NQGetMin(n, len + 1);
  memcpy(buffer, path, sz);
  return len;
}

size_t NQWinPathFrom(NQWChar* buffer, size_t n, const char* path)
{
  if (n == 0)
    return 0;

  NQUChar character;
  const uint8_t* utf8Start = (const uint8_t*)path;
  uint16_t* utf16Start = (uint16_t*)buffer;
  uint16_t* utf16End = utf16Start + n;

  while (*utf8Start != '\0') {
    character = NQUCharNext8(utf8Start, utf8Start + 6, &utf8Start);
    if (!NQIsUChar(character)) {
      NQ_ASSERT(0);
      *buffer = '\0';
      return 0; // TODO: return length
    }

    if (character == NQ_PATH_DELIMITER)
      character = NQ_WINPATH_DELIMITER;

    if (!NQUCharPush16(utf16Start, utf16End, &utf16Start, character)) {
      NQ_ASSERT(0);
      *buffer = '\0';
      return 0; // TODO: return length
    }
  }
  
  if (utf16Start < utf16End)
    *utf16Start = '\0';

  return utf16Start - (uint16_t*)buffer;
}

size_t NQGetAbsoluteWinPath(NQWChar* buffer, size_t n, const char* path)
{
#ifdef NQ_OS_WINDOWS
  WCHAR winpath[MAX_PATH];
  size_t length = NQWinPathFrom(winpath, sizeof(winpath), path);
  if (length != 0 && length < NQ_ARRAY_LENGTH(winpath) && n <= MAXDWORD)
    return (size_t)GetFullPathNameW(winpath, (DWORD)n, buffer, NULL);
#endif

  return 0;
}

bool NQIsAbsolutePath(const char* path) {
  if (path[0] == NQ_PATH_DELIMITER)
    return true;
  if (path[0] == '\0')
    return false;
  // TODO: IsAlpha(path[0])
  if (path[1] == ':')
    return true;
  return false;
}

const char* NQGetFilename(const char* path)
{
  const char* filename = path;
  const char* p = path;
  while (*p) {
    if (*p++ == NQ_PATH_DELIMITER) {
      filename = p;
    }
  }
  return filename;
}
