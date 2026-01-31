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

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_PATH_DELIMITER '/'
#define NQ_WINPATH_DELIMITER '\\'

#define NQIsPathDelimiter(c) ((c) == NQ_PATH_DELIMITER)

typedef struct NQPath NQPath;
struct NQPath {
  uint16_t length;
  char characters[1];
};

NQ_EXPORT NQPath* NQPath_create(const char* path);
NQ_EXPORT NQPath* NQPath_fromJoin2(const char* path1, const char* path2);
NQ_EXPORT void NQPath_destroy(NQPath*);

#define NQPath_characters(thiz) (thiz)->characters
static inline size_t NQPath_length(NQPath* thiz)
{
  return thiz->length;
}

typedef struct NQPathBuilder NQPathBuilder;
struct NQPathBuilder {
  char* characters;
  uint16_t length;
  uint16_t capacity;
  char buffer[80];
};

NQ_EXPORT void NQPathBuilder_init(NQPathBuilder*);
NQ_EXPORT bool NQPathBuilder_initJoin1(NQPathBuilder*, const char* path1);
NQ_EXPORT bool NQPathBuilder_initJoin2(NQPathBuilder*, const char* path1, const char* path2);
NQ_EXPORT bool NQPathBuilder_initJoin3(NQPathBuilder*, const char* path1, const char* path2, const char* path3);
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

NQ_EXPORT size_t NQPathFrom(char* buffer, size_t n, const NQWChar* path);
NQ_EXPORT size_t NQGetAbsolutePath(char* buffer, size_t n, const char* path);
NQ_EXPORT size_t NQWinPathFrom(NQWChar* buffer, size_t n, const char* path);
NQ_EXPORT size_t NQGetAbsoluteWinPath(NQWChar* buffer, size_t n, const char* path);
NQ_EXPORT bool NQIsAbsolutePath(const char* path);
NQ_EXPORT const char* NQGetFilename(const char* path);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_PATH_H */
