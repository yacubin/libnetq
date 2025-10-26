/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_URLPATH_H
#define _LIBNETQ_URLPATH_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQUrlPath NQUrlPath;

NQ_EXPORT NQUrlPath* NQUrlPath_create(const char* path, const char* pattern, bool onlymatch);
NQ_EXPORT void NQUrlPath_destroy(NQUrlPath*);
NQ_EXPORT size_t NQUrlPath_segmentCount(const NQUrlPath*);
NQ_EXPORT const char* NQUrlPath_segmentAt(const NQUrlPath*, size_t index);
NQ_EXPORT const char* NQUrlPath_segment(const NQUrlPath*, const char* name);
NQ_EXPORT const char* NQUrlPath_path(const NQUrlPath*);
NQ_EXPORT bool NQUrlPath_isAbsolute(const NQUrlPath*);

NQ_EXPORT bool NQIsUrlPathPattern(const char* pattern);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_URLPATH_H */
