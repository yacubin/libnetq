/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_PATH_H
#define _LIBNETQ_PATH_H

#include <libnetq/Basic.h>
#include <libnetq/string/StringArray.h>
#include <libnetq/string/StringRange.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_PATH_DELIMITER '/'
#define NQ_WINPATH_DELIMITER '\\'

#define NQIsPathDelimiter(c) ((c) == NQ_PATH_DELIMITER)

typedef struct NQStringArray16 NQPath;

NQ_EXPORT NQPath* NQPath_create(const char* path);
NQ_EXPORT NQPath* NQPath_fromJoin2(const char* path1, const char* path2);

static inline void NQPath_destroy(NQPath* thiz)
{
  NQStringArray16_destroy(thiz);
}

#define NQPath_characters(thiz) NQStringArray16_characters(thiz)

static inline size_t NQPath_length(const NQPath* thiz)
{
  return NQStringArray16_length(thiz);
}

typedef struct NQPathBuilder NQPathBuilder;
struct NQPathBuilder {
  char* characters;
  uint16_t length;
  uint16_t capacity;
  char buffer[80];
};

NQ_EXPORT void NQPathBuilder_init(NQPathBuilder*);
NQ_EXPORT bool NQPathBuilder_initPath(NQPathBuilder*, const char* path1);
NQ_EXPORT bool NQPathBuilder_initJoin2(NQPathBuilder*, const char* path1, const char* path2);
NQ_EXPORT bool NQPathBuilder_initJoin3(NQPathBuilder*, const char* path1, const char* path2, const char* path3);
NQ_EXPORT bool NQPathBuilder_initResolve2(NQPathBuilder*, const char* path1, const char* path2);
NQ_EXPORT bool NQPathBuilder_initResolve3(NQPathBuilder*, const char* path1, const char* path2, const char* path3);
NQ_EXPORT void NQPathBuilder_finalize(NQPathBuilder*);

#define NQPathBuilder_characters(thiz) (thiz)->characters
static inline size_t NQPathBuilder_length(NQPathBuilder* thiz)
{
  return thiz->length;
}

NQ_EXPORT void NQPathBuilder_clear(NQPathBuilder*, const char* path);
NQ_EXPORT bool NQPathBuilder_join(NQPathBuilder*, const char* path);
NQ_EXPORT bool NQPathBuilder_add(NQPathBuilder*, const char* text);
NQ_EXPORT void NQPathBuilder_removeLastSegment(NQPathBuilder*);

typedef struct NQWinPathBuilder NQWinPathBuilder;
struct NQWinPathBuilder {
  NQWChar* characters;
  uint16_t length;
  uint16_t capacity;
  NQWChar buffer[80];
};

NQ_EXPORT void NQWinPathBuilder_init(NQWinPathBuilder*);

typedef struct NQPathInfo NQPathInfo;
struct NQPathInfo {
  NQStringRange path;
  NQStringRange dirname;
  NQStringRange basename;
  bool isAbsolute;
  bool isDirOnly;
  bool isNormalize;
};

NQ_EXPORT bool NQPathInfoParse(const char* path, NQPathInfo* result);
NQ_EXPORT bool NQPathInfoParse2(const char* path, size_t length, NQPathInfo* result);

NQ_EXPORT size_t NQPathFrom(char* buffer, size_t n, const NQWChar* path);
NQ_EXPORT size_t NQGetAbsolutePath(char* buffer, size_t n, const char* path);
NQ_EXPORT size_t NQWinPathFrom(NQWChar* buffer, size_t n, const char* path);
NQ_EXPORT size_t NQGetAbsoluteWinPath(NQWChar* buffer, size_t n, const char* path);

static inline bool NQIsAbsolutePosixPath(const char* path)
{
  return path[0] == NQ_PATH_DELIMITER;
}

static inline bool NQIsRootPosixPath(const char* path)
{
  return path[0] == NQ_PATH_DELIMITER && path[1] == '\0';
}

NQ_EXPORT bool NQIsAbsolutePath(const char* path);
NQ_EXPORT const char* NQGetFilename(const char* path);
NQ_EXPORT const char* NQGetExtname(const char* path);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_PATH_H */
