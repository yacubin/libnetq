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
#include <libnetq/string/String.h>
#include <libnetq/MinMax.h>
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

static inline bool isPathSegmentDelimiter(char ch)
{
  return ch == NQ_PATH_DELIMITER || ch == NQ_WINPATH_DELIMITER;
}

struct SegmentIterator {
  const char* path;
  const char* segmentData;
  size_t segmentSize;
  bool withStartDelimitter;
};

static void SegmentIterator_init(struct SegmentIterator* thiz, const char* path)
{
  thiz->path = path;
  thiz->segmentData = path;
  thiz->segmentSize = 0;
  thiz->withStartDelimitter = false;
}

static bool SegmentIterator_next(struct SegmentIterator* thiz)
{
  if (*thiz->path == '\0')
    return false;

  thiz->withStartDelimitter = false;
  while (*thiz->path && isPathSegmentDelimiter(*thiz->path)) {
    thiz->withStartDelimitter = true;
    thiz->path++;
  }

  thiz->segmentData = thiz->path;
  while (*thiz->path && !isPathSegmentDelimiter(*thiz->path))
    thiz->path++;

  thiz->segmentSize = thiz->path - thiz->segmentData;
  return true;
}

struct SegmentWriter {
  char* buffer;
  char* bufferEnd;
  bool isAbsolute;
  size_t segmentCount;
  size_t pathLength;
};

static void SegmentWriter_init(struct SegmentWriter* thiz, char* buffer, size_t* length)
{
  thiz->buffer = buffer;
  if (buffer == NULL)
    thiz->bufferEnd = NULL;
  else if (length)
    thiz->bufferEnd = buffer + *length;
  else
    thiz->bufferEnd = (char*)(~(size_t)0);
  thiz->isAbsolute = false;
  thiz->segmentCount = 0;
  thiz->pathLength = 0;
}

static void SegmentWriter_add(struct SegmentWriter* thiz, bool hasDelimitter, const char* segment, size_t length)
{
  if (thiz->segmentCount == 0 && hasDelimitter) {
    if (thiz->buffer < thiz->bufferEnd)
      *thiz->buffer++ = NQ_PATH_DELIMITER;
    thiz->pathLength++;
    thiz->isAbsolute = true;
  }

  thiz->segmentCount++;

  if (length == 0 || (length == 1 && segment[0] == '.'))
    return;

  if (length == 2 && segment[0] == '.' && segment[1] == '.') {
    // TODO: ".."
    return;
  }

  if (thiz->pathLength > 0 && (thiz->pathLength != 1 || !thiz->isAbsolute)) {
    if (thiz->buffer < thiz->bufferEnd)
      *thiz->buffer++ = NQ_PATH_DELIMITER;
    thiz->pathLength++;
  }

  size_t size = NQGetMin(length, thiz->bufferEnd - thiz->buffer);
  if (size != 0) {
    memmove(thiz->buffer, segment, size);
    thiz->buffer += size;
  }

  thiz->pathLength += length;
}

static NQ_ALWAYS_INLINE
void pathJoin1(const char* path1, char* buf, size_t* len)
{
  struct SegmentWriter wr;
  SegmentWriter_init(&wr, buf, len);

  struct SegmentIterator sr;
  SegmentIterator_init(&sr, path1);

  while (SegmentIterator_next(&sr))
    SegmentWriter_add(&wr, sr.withStartDelimitter, sr.segmentData, sr.segmentSize);

  if (len != NULL)
    *len = wr.pathLength;
}

static NQ_ALWAYS_INLINE
void pathJoin2(const char* path1, const char* path2, char* buf, size_t* len)
{
  struct SegmentWriter wr;
  SegmentWriter_init(&wr, buf, len);

  struct SegmentIterator sr;

  SegmentIterator_init(&sr, path1);
  while (SegmentIterator_next(&sr))
    SegmentWriter_add(&wr, sr.withStartDelimitter, sr.segmentData, sr.segmentSize);

  SegmentIterator_init(&sr, path2);
  while (SegmentIterator_next(&sr))
    SegmentWriter_add(&wr, sr.withStartDelimitter, sr.segmentData, sr.segmentSize);

  if (len != NULL)
    *len = wr.pathLength;
}

static NQ_ALWAYS_INLINE
void pathJoin3(const char* path1, const char* path2, const char* path3, char* buf, size_t* len)
{
  struct SegmentWriter wr;
  SegmentWriter_init(&wr, buf, len);

  struct SegmentIterator sr;

  SegmentIterator_init(&sr, path1);
  while (SegmentIterator_next(&sr))
    SegmentWriter_add(&wr, sr.withStartDelimitter, sr.segmentData, sr.segmentSize);
  SegmentIterator_init(&sr, path2);
  while (SegmentIterator_next(&sr))
    SegmentWriter_add(&wr, sr.withStartDelimitter, sr.segmentData, sr.segmentSize);
  SegmentIterator_init(&sr, path3);
  while (SegmentIterator_next(&sr))
    SegmentWriter_add(&wr, sr.withStartDelimitter, sr.segmentData, sr.segmentSize);

  if (len != NULL)
    *len = wr.pathLength;
}

static NQ_ALWAYS_INLINE
void pathResolve2(const char* path1, const char* path2, char* buf, size_t* len)
{
  if (NQIsAbsolutePath(path2))
    pathJoin1(path2, buf, len);
  else
    pathJoin2(path1, path2, buf, len);
}

static NQ_ALWAYS_INLINE
void pathResolve3(const char* path1, const char* path2, const char* path3, char* buf, size_t* len)
{
  if (NQIsAbsolutePath(path3))
    pathJoin1(path3, buf, len);
  else if (NQIsAbsolutePath(path2))
    pathJoin2(path2, path3, buf, len);
  else
    pathJoin3(path1, path2, path3, buf, len);
}

NQPath* NQPath_create(const char* path)
{
  size_t length;
  pathJoin1(path, NULL, &length);
  NQPath* thiz = NQStringArray16_alloc(length);
  if (thiz == NULL)
    return NULL;
  pathJoin1(path, thiz->characters, &length);
  return thiz;
}

NQPath* NQPath_fromJoin2(const char* path1, const char* path2)
{
  size_t length;
  pathJoin2(path1, path2, NULL, &length);
  NQPath* thiz = NQStringArray16_alloc(length);
  if (thiz == NULL)
    return NULL;
  pathJoin2(path1, path2, thiz->characters, &length);
  return thiz;
}

NQPath* NQPath_fromResolve2(const char* path1, const char* path2)
{
  size_t length;
  pathResolve2(path1, path2, NULL, &length);
  NQPath* thiz = NQStringArray16_alloc(length);
  if (thiz == NULL)
    return NULL;
  pathResolve2(path1, path2, thiz->characters, &length);
  return thiz;
}

static inline void pathBuilderInit(NQPathBuilder* thiz)
{
  thiz->characters = thiz->buffer;
  thiz->characters[0] = '\0';
  thiz->length = 0;
  thiz->capacity = sizeof(thiz->buffer);
}

static inline void pathBuilderFinalize(NQPathBuilder* thiz)
{
  if (thiz->characters != thiz->buffer)
    NQFree(thiz->characters);
}

static bool pathBuilderReserveCapacity(NQPathBuilder* thiz, size_t newCapacity)
{
  if (NQ_UINT16_MAX < newCapacity)
    return false;

  if (newCapacity <= thiz->capacity)
    return true;

  char* oldCharacters = thiz->characters;
  char* newCharacters = (char*)NQMalloc(newCapacity);
  if (newCharacters == NULL)
    return false;

  memcpy(newCharacters, oldCharacters, thiz->length);
  if (oldCharacters != thiz->buffer) {
    NQFree(oldCharacters);
  }

  thiz->characters = newCharacters;
  thiz->capacity = (uint16_t)newCapacity;

  return true;
}

static inline size_t pathBuilderNextCapacity(NQPathBuilder* thiz, size_t newMinCapacity)
{
  return NQGetMax(newMinCapacity, thiz->capacity + thiz->capacity / 4 + 1);
}

static inline bool pathBuilderExpandCapacity(NQPathBuilder* thiz, size_t newMinCapacity)
{
  return pathBuilderReserveCapacity(thiz, pathBuilderNextCapacity(thiz, newMinCapacity));
}

void NQPathBuilder_init(NQPathBuilder* thiz)
{
  pathBuilderInit(thiz);
}

bool NQPathBuilder_initPath(NQPathBuilder* thiz, const char* path1)
{
  pathBuilderInit(thiz);

  size_t length;
  pathJoin1(path1, NULL, &length);
  if (!pathBuilderReserveCapacity(thiz, length + 1))
    return false;

  pathJoin1(path1, thiz->characters, &length);
  thiz->characters[length] = '\0';
  thiz->length = (uint16_t)length;
  return true;
}

bool NQPathBuilder_initJoin2(NQPathBuilder* thiz, const char* path1, const char* path2)
{
  pathBuilderInit(thiz);

  size_t length;
  pathJoin2(path1, path2, NULL, &length);
  if (!pathBuilderReserveCapacity(thiz, length + 1))
    return false;

  pathJoin2(path1, path2, thiz->characters, &length);
  thiz->characters[length] = '\0';
  thiz->length = (uint16_t)length;
  return true;
}

bool NQPathBuilder_initJoin3(NQPathBuilder* thiz, const char* path1, const char* path2, const char* path3)
{
  pathBuilderInit(thiz);

  size_t length;
  pathJoin3(path1, path2, path3, NULL, &length);
  if (!pathBuilderReserveCapacity(thiz, length + 1))
    return false;

  pathJoin3(path1, path2, path3, thiz->characters, &length);
  thiz->characters[length] = '\0';
  thiz->length = (uint16_t)length;
  return true;
}

bool NQPathBuilder_initResolve2(NQPathBuilder* thiz, const char* path1, const char* path2)
{
  pathBuilderInit(thiz);

  size_t length;
  pathResolve2(path1, path2, NULL, &length);
  if (!pathBuilderReserveCapacity(thiz, length + 1))
    return false;

  pathResolve2(path1, path2, thiz->characters, &length);
  thiz->characters[length] = '\0';
  thiz->length = (uint16_t)length;
  return true;
}

bool NQPathBuilder_initResolve3(NQPathBuilder* thiz, const char* path1, const char* path2, const char* path3)
{
  pathBuilderInit(thiz);

  size_t length;
  pathResolve3(path1, path2, path3, NULL, &length);
  if (!pathBuilderReserveCapacity(thiz, length + 1))
    return false;

  pathResolve3(path1, path2, path3, thiz->characters, &length);
  thiz->characters[length] = '\0';
  thiz->length = (uint16_t)length;
  return true;
}

void NQPathBuilder_finalize(NQPathBuilder* thiz)
{
  pathBuilderFinalize(thiz);
}

void NQPathBuilder_clear(NQPathBuilder* thiz, const char* path)
{
  pathBuilderFinalize(thiz);
  pathBuilderInit(thiz);
}

bool NQPathBuilder_join(NQPathBuilder* thiz, const char* path)
{
  size_t length;
  pathJoin2(thiz->characters, path, NULL, &length);
  if (!pathBuilderReserveCapacity(thiz, length + 1))
    return false;

  pathJoin2(thiz->characters, path, thiz->characters, &length);
  thiz->characters[length] = '\0';
  thiz->length = (uint16_t)length;
  return true;
}

bool NQPathBuilder_add(NQPathBuilder* thiz, const char* text)
{
  size_t length = NQStrlen(text);
  if (length == 0)
    return true;

  size_t newSize = thiz->length + length + 1;
  if (newSize < thiz->length)
    return false;

  if (newSize > thiz->capacity) {
    if (!pathBuilderExpandCapacity(thiz, newSize))
      return false;
  }

  char* ptr = thiz->characters + thiz->length;
  for (size_t i = 0; i < length; i++)
    *ptr++ = normalizeCharacter(text[i]);
  *ptr = '\0';

  thiz->length = (uint16_t)(ptr - thiz->characters);
  return true;
}

void NQPathBuilder_removeLastSegment(NQPathBuilder* thiz)
{
  uint16_t length = thiz->length;
  while (length) {
    char ch = thiz->characters[--length];
    if (ch == NQ_PATH_DELIMITER) {
      thiz->characters[length] = '\0';
      thiz->length = length;
      return;
    }
  }
}

void NQWinPathBuilder_init(NQWinPathBuilder* thiz)
{
  thiz->characters = thiz->buffer;
  thiz->characters[0] = L'\0';
  thiz->length = 0;
  thiz->capacity = NQ_ARRAY_LENGTH(thiz->buffer);
}

bool NQPathInfoParse(const char* path, NQPathInfo* result)
{
  return NQPathInfoParse2(path, strlen(path), result);
}

bool NQPathInfoParse2(const char* path, size_t length, NQPathInfo* result)
{
  if (length == 0) {
    result->path.characters = path;
    result->path.length = 0;
    result->dirname.characters = ".";
    result->dirname.length = 1;
    result->basename.characters = path;
    result->basename.length = 0;
    result->isAbsolute = false;
    result->isDirOnly = false;
    result->isNormalize = true;
    return true;
  }

  bool isNormalize = true;
  char lastChar = path[0];

  const char* basename = path;
  bool isAbsolute = (lastChar == NQ_PATH_DELIMITER);
  int normalizeCount = (lastChar == '.') ? 1 : 3;

  for (size_t i = 1; i < length; i++) {
    char ch = path[i];
    if (ch == '\0') {
      break;
    }
    if (ch != NQ_PATH_DELIMITER) {
      if (lastChar == NQ_PATH_DELIMITER) {
        basename = &path[i];
        normalizeCount = (ch == '.') ? 1 : 3;
      }
      else if (ch == '.') {
        normalizeCount++;
      }
    }
    else if (lastChar == NQ_PATH_DELIMITER || normalizeCount < 3) {
      isNormalize = false;
    }
    lastChar = ch;
  }

  if (length == 1 && lastChar == NQ_PATH_DELIMITER) {
    result->path.characters = path;
    result->path.length = 1;
    result->dirname.characters = path;
    result->dirname.length = 1;
    result->basename.characters = path;
    result->basename.length = 0;
    result->isAbsolute = isAbsolute;
    result->isDirOnly = true;
    result->isNormalize = isNormalize;
    return true;
  }

  result->path.characters = path;
  result->path.length = length;
  result->dirname.characters = path;
  result->dirname.length = basename - path;
  if (result->dirname.length > 1)
    result->dirname.length--;
  if (result->dirname.length == 0) {
    result->dirname.characters = ".";
    result->dirname.length = 1;
  }
  result->basename.characters = basename;
  result->basename.length = length - (basename - path);
  result->isAbsolute = isAbsolute;
  result->isDirOnly = (lastChar == NQ_PATH_DELIMITER);
  if (result->isDirOnly)
    result->basename.length--;
  result->isNormalize = isNormalize;

  return true;
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
  size_t len = NQStrlen(path);
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
  if (NQIsAbsolutePosixPath(path))
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

const char* NQGetExtname(const char* path)
{
  return NQStrrchr(path, '.');
}
