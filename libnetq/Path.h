/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
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

// NQPathAppendComponent
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
